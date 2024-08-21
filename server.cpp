#include <uds_server.hpp>

#include "mtp_server.hpp"
#include <iostream>
#include <message_handler.hpp>

int main() {
    try {
        auto server = std::make_unique<MTP::MTPServer>(std::make_unique<MTP::UDSServer>(MTP::Constants::UDS_PATH));
        server->run();
        return 0;
    }
    catch (std::exception &e) {
        std::cout << e.what() << std::endl;
    }
}
