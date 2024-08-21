//
// Created by johnny on 8/20/24.
//
#pragma once
#include "ipc_server.hpp"
#include <memory>

namespace MTP {
    class UDSServer : public IPCServer {
    public:
        explicit UDSServer(const std::string& socket_path);
        ~UDSServer() override;

        /**
         * @return Returns a unique_ptr of IPCChannel for every incoming connection.
         */
        std::unique_ptr<MTP::IPCChannel> accept_connection() override;

    private:
        int m_primary_socket_fd;
        std::string m_socket_path;
    };
}
