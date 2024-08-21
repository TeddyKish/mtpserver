cmake_minimum_required(VERSION 3.10)

# Set the project name
project(DI_TASK)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add executable target for the server
add_executable(server
        uds_channel.cpp uds_server.cpp message_handler.cpp mtp_server.cpp server.cpp user_data_handler.cpp)

# Add executable target for the client
add_executable(client
        client.cpp message_handler.cpp uds_channel.cpp)

target_compile_options(server PRIVATE -Wall -Wextra)
target_compile_options(client PRIVATE -Wall -Wextra)
include_directories(${PROJECT_SOURCE_DIR})
