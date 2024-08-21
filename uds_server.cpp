//
// Created by johnny on 8/20/24.
//

#include "uds_server.hpp"
#include "uds_channel.hpp"

namespace MTP {
    namespace Constants {
        constexpr int MAX_QUEUED_CONNECTIONS = 10;
    }

    UDSServer::UDSServer(const std::string& socket_path) :
     m_primary_socket_fd(Constants::INACTIVE_SOCKET_VALUE) , m_socket_path(socket_path) {
        m_primary_socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
        if (m_primary_socket_fd < 0) {
            throw MTPException("The server failed to create the primary socket");
        }

        struct sockaddr_un addr;
        addr.sun_family = AF_UNIX;
        strncpy(addr.sun_path, socket_path.c_str(), sizeof(addr.sun_path) - 1);

        // Remove any previous socket file
        unlink(socket_path.c_str());
        int res = bind(m_primary_socket_fd, (struct sockaddr*)&addr, sizeof(addr));

        if (res < 0) {
            throw MTPException("The server failed to bind the primary socket");
        }

        res = listen(m_primary_socket_fd, Constants::MAX_QUEUED_CONNECTIONS);
        if (res < 0) {
            perror("listen");
            throw MTPException("The server failed to listen on the primary socket");
        }
    }

    UDSServer::~UDSServer() {
        if (m_primary_socket_fd != Constants::INACTIVE_SOCKET_VALUE) {
            close(m_primary_socket_fd);
            unlink(m_socket_path.c_str());
        }
    }

    std::unique_ptr<IPCChannel> UDSServer::accept_connection() {
        int client_fd = accept(m_primary_socket_fd, nullptr, nullptr);
        if (client_fd < 0) {
            throw MTPException("The server failed to accept a connection");
        }

        return std::make_unique<UDSChannel>(client_fd);
    }
}
