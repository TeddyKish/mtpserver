//
// Created by johnny on 8/20/24.
//
#pragma once
#include <string>

namespace MTP {
    /**
     * Represents a single communication channel.
     */
    class IPCChannel {
    public:
        IPCChannel() = default;
        virtual ~IPCChannel() = default;

        /**
         * Transmits <msg> across the channel.
         * @param msg The message to send.
         */
        virtual void send_message(const std::string& msg) const = 0;

        /**
         * Receives a message from the channel.
         * @return A std::string that contains the content.
         */
        virtual std::string receive_message() const = 0;

        /**
         * @return The UID of the peer.
         */
        virtual uid_t get_peer_uid() const = 0;
    };
}
