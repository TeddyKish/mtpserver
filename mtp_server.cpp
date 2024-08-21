//
// Created by johnny on 8/20/24.
//
#include <iostream>
#include <algorithm>
#include "mtp_server.hpp"

#include <message_handler.hpp>

namespace MTP {
    MTPServer::MTPServer(std::unique_ptr<IPCServer> server)
        : m_server(std::move(server)) {}

    MTPServer::~MTPServer() {
        {
            std::lock_guard<std::mutex> lock(m_threads_mutex);
            for (std::thread& t : m_threads) {
                if (t.joinable()) {
                    t.join();
                }
            }
            m_threads.clear();
        }
    }

    void MTPServer::run() {
        while (true) {
            auto conn = m_server->accept_connection();
            std::lock_guard<std::mutex> lock(m_threads_mutex);
            m_threads.emplace_back(&MTPServer::handle_client, this, std::move(conn));
        }
    }

    void MTPServer::handle_client(std::unique_ptr<IPCChannel>&& channel_)
    {
        try {
            auto channel(std::move(channel_));
            while (true) {
                std::string msg = channel->receive_message();

                switch(MessageHandler::get_message_type(msg)) {
                    case Types::APPEND_STRING: {
                        std::string to_sort = std::string(MessageHandler::get_message_content(msg));
                        const auto peer_uid = channel->get_peer_uid();

                        m_user_data_handler.append_chunk(peer_uid, to_sort);
                        break;
                    }

                    case Types::REQUEST_CHUNK: {
                        std::string chunk_size_str = std::string(MessageHandler::get_message_content(msg));
                        if (!MessageHandler::is_numeric(chunk_size_str)) {
                            throw MTPException("Received illegal request-chunk message content");
                        }

                        size_t requested_chunk_size = std::stoull(chunk_size_str);
                        const auto peer_uid = channel->get_peer_uid();

                        std::string requested_chunk = m_user_data_handler.extract_chunk(peer_uid, requested_chunk_size);
                        channel->send_message(MessageHandler::generate_message(Types::RESPONSE_CHUNK, requested_chunk));
                        break;
                    }

                    default:
                        throw MTPException("Unsupported Message Type received from client");
                }
            }
        }
        catch (std::exception& e) {
            std::cout << e.what() << std::endl;
        }
    }
}
