//
// Created by johnny on 8/20/24.
//
#include "uds_channel.hpp"

#include <message_handler.hpp>

namespace MTP {

    UDSChannel::UDSChannel(const std::string& socket_path) :
    m_socket_path(socket_path), m_socket_fd(Constants::INACTIVE_SOCKET_VALUE) {
        if ((m_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0)) == -1) {
            throw MTP::MTPException("Failed initializing a socket within UDS");
        }

        sockaddr_un addr{};
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, m_socket_path.c_str(), sizeof(addr.sun_path) - 1);

        if (connect(m_socket_fd, (struct sockaddr*)&addr, sizeof(addr)) == -1) {
            close(m_socket_fd);
            m_socket_fd = Constants::INACTIVE_SOCKET_VALUE;
            throw MTP::MTPException("Failed connecting to the socket within UDS");
        }
    }

    UDSChannel::UDSChannel(int socket_fd) : m_socket_fd(socket_fd) {
        if (m_socket_fd <= 0) {
            throw MTP::MTPException("Received invalid socket fd when initializing a connection to the UDS");
        }
    }

    UDSChannel::~UDSChannel() {
        if (m_socket_fd != Constants::INACTIVE_SOCKET_VALUE) {
            close(m_socket_fd);
            m_socket_fd = Constants::INACTIVE_SOCKET_VALUE;
        }
    }

    void UDSChannel::send_message(const std::string& msg) const
    {
        if (msg.empty()) {
            throw MTP::MTPException("Can't transmit an empty string");
        }

        // Send data to the socket
        ssize_t bytes_sent = send(m_socket_fd, msg.c_str(), msg.size(), 0);

        // Check for errors
        if (bytes_sent < 0) {
            throw MTP::MTPException("Error occured trying to send information on the socket");
        } else if (bytes_sent == 0) {
            throw MTP::MTPException("No information was sent successfully on the socket");
        }
    }

    std::string UDSChannel::receive_message() const
    {
        std::string res;
        ssize_t bytes_received;
        char byte_received = '\0';

        // Read bytes one-by-one, as the input is a stream
        while (byte_received != MTP::Constants::MESSAGE_DELIMITER) {
            // Receive data from the socket
            bytes_received = recv(m_socket_fd, &byte_received, 1, 0);

            // Check for errors or if the connection is closed
            if (bytes_received < 0) {
                throw MTP::MTPException("Failed receiving information from socket");
            } else if (bytes_received == 0) {
                throw MTP::MTPException("UDSChannel Connection ended");
            }

            res += byte_received;
        }

        return res;
    }

    uid_t UDSChannel::get_peer_uid() const
    {
        struct ucred cred;
        socklen_t len = sizeof(cred);
        if (getsockopt(m_socket_fd, SOL_SOCKET, SO_PEERCRED, &cred, &len) == 0) {
            return cred.uid;
        } else {
            throw MTP::MTPException("getsockopt failed to retrieve socket information");
        }
    }
}
