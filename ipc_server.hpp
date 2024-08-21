//
// Created by johnny on 8/20/24.
//
#pragma once

#include "ipc_channel.hpp"
#include <memory>

namespace MTP {
    class IPCServer {
    public:
        IPCServer() = default;
        virtual ~IPCServer() = default;

        /**
         * Accept incoming connections to the server.
         * @return A unique_ptr to the connection, represented generally as an IPCChannel.
         */
        virtual std::unique_ptr<MTP::IPCChannel> accept_connection() = 0;
    };
}
