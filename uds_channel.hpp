#pragma once
#include "ipc_channel.hpp"
#include "mtp_exception.hpp"
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
#include <cstring>
#include <iostream>

namespace MTP {
    namespace Constants {
        constexpr auto INACTIVE_SOCKET_VALUE = -1;
        constexpr auto RESPONSE_BUFFER_SIZE = 1024;
        constexpr auto UDS_PORT = 1337;
        constexpr auto UDS_SOCK_PATH = "/tmp/uds";
        constexpr auto UDS_SOCK_PATH_LEN = sizeof(UDS_SOCK_PATH);
    }

    class UDSChannel final : public IPCChannel {
    public:
        explicit UDSChannel(const std::string& socket_path);
        explicit UDSChannel(int socket_fd);
        ~UDSChannel() override;

        /**
         * Sends <msg> through the UDS.
         * @param msg The message to send.
         */
        void send_message(const std::string& msg) const override;

        /**
         * Receives a message from the stream-based socket and returns it.
         * Reads all bytes in the socket up until the first MTP::Constants::MTP_DELIMITER, including it.
         * @return
         */
        std::string receive_message() const override;

        /**
         * Utilizes the properties of the socket to return the peer's UID.
         * @return The peer's UID.
         */
        uid_t get_peer_uid() const override;

    private:
        std::string m_socket_path;
        int m_socket_fd;
    };
}
