#pragma once

#include <memory>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <map>

#include "ipc_server.hpp"
#include "ipc_channel.hpp"
#include "user_data_handler.hpp"

namespace MTP {
    class MTPServer {
    public:
        explicit MTPServer(std::unique_ptr<IPCServer> server);
        ~MTPServer();

        /**
         * Performs the logic of the server.
         * Accepts connections and spawns a unique thread for each one.
         */
        void run();

    private:
        /**
         * Handles a single client represented by the input parameter.
         * @param channel_ The IPCChannel of the specific client.
         */
        void handle_client(std::unique_ptr<IPCChannel>&& channel_);

        UserDataHandler m_user_data_handler;
        std::unique_ptr<IPCServer> m_server;
        std::vector<std::thread> m_threads;
        std::mutex m_threads_mutex;
    };
}
