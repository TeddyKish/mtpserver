#include <memory>

#include "ipc_channel.hpp"
#include "uds_channel.hpp"
#include "message_handler.hpp"

int main() {
    try {
        std::unique_ptr<MTP::IPCChannel> channel = std::make_unique<MTP::UDSChannel>(MTP::Constants::UDS_PATH);

        while(true) {
            sleep(2);

            channel->send_message(MTP::MessageHandler::generate_message(MTP::Types::APPEND_STRING, "hello"));
            channel->send_message(MTP::MessageHandler::generate_message(MTP::Types::REQUEST_CHUNK, "5"));
            auto response = MTP::MessageHandler::get_message_content(channel->receive_message());
            std::cout << "Response was" << response << std::endl;  // Should print "ehllo"

            channel->send_message(MTP::MessageHandler::generate_message(MTP::Types::APPEND_STRING, "hello"));
            channel->send_message(MTP::MessageHandler::generate_message(MTP::Types::REQUEST_CHUNK, "2"));
            response = MTP::MessageHandler::get_message_content(channel->receive_message());
            std::cout << "Response was" << response << std::endl;  // Should print "eh"

            channel->send_message(MTP::MessageHandler::generate_message(MTP::Types::APPEND_STRING, "world"));
            channel->send_message(MTP::MessageHandler::generate_message(MTP::Types::REQUEST_CHUNK, "8"));
            response = MTP::MessageHandler::get_message_content(channel->receive_message());
            std::cout << "Response was" << response << std::endl;  // Should print "dllloorw"
        }
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }

    return 0;
}
