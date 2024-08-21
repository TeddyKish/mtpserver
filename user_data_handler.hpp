//
// Created by johnny on 8/20/24.
//

#pragma once
#include <string>
#include <map>
#include <mutex>

namespace MTP {
    class UserDataHandler {
    public:
        UserDataHandler() = default;
        ~UserDataHandler() = default;

        /**
         * Appends the represented chunk to the internal data structures.
         * Performs sorting on the data, such that every stored content is sorted.
         * @param uid The UID of the user.
         * @param content The content to append.
         */
        void append_chunk(uid_t uid, const std::string& content);

        /**
         * Extracts a chunk and returns it in the form of std::string.
         * @param uid The UID of the user to extract data from.
         * @param requested_chunk_size The size of the requested chunk.
         * @return A string that represents the chunk.
         */
        std::string extract_chunk(uid_t uid, size_t requested_chunk_size);

    private:
        std::map<uid_t, std::string> m_user_data;
        std::mutex m_connections_mutex; // For ensuring safe access to the map
    };

}
