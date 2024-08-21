//
// Created by johnny on 8/20/24.
//

#include "user_data_handler.hpp"
#include "mtp_exception.hpp"
#include <algorithm>

namespace MTP {
    void UserDataHandler::append_chunk(uid_t uid, const std::string& content) {
        std::lock_guard<std::mutex> lock(m_connections_mutex);
        const auto& it = m_user_data.find(uid);
        std::string to_sort = content;

        // If the uid already exists, append the previous result to the to-be-sorted data
        if (it != m_user_data.end()) {
            to_sort += it->second;
        }

        std::sort(to_sort.begin(), to_sort.end());
        m_user_data[uid] = to_sort;
    }

    std::string UserDataHandler::extract_chunk(uid_t uid, size_t requested_chunk_size) {
        std::lock_guard<std::mutex> lock(m_connections_mutex);
        const auto& it = m_user_data.find(uid);

        // If the uid doesn't exist, this is illegal
        if (it == m_user_data.end()) {
            throw MTPException("Received chunk retrieval request from a first-time user");
        }

        std::string& stored_string = it->second;
        if (stored_string.size() < requested_chunk_size) {
            throw MTPException("Received a chunk size that exceeds the entire stored string's size");
        }

        std::string requested_chunk = stored_string.substr(0, requested_chunk_size);
        stored_string.erase(0, requested_chunk_size);

        return requested_chunk;
    }
}