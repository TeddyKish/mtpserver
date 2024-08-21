#include "message_handler.hpp"
#include "mtp_exception.hpp"

namespace MTP {
    namespace Constants {
        constexpr const char* APPEND_STRING_PREFIX = "S";
        constexpr const char* REQUEST_CHUNK_PREFIX = "Q";
        constexpr const char* RESPONSE_CHUNK_PREFIX = "A";
        constexpr uint8_t PREFIX_LENGTH = 1;
        constexpr uint8_t MINIMUM_CONTENT_LENGTH = 1;
    }

    bool MessageHandler::is_alphabetic(const std::string& str) {
        // Check if all characters in the string are alphabetic
        return std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return std::isalpha(c);
        });
    }

    bool MessageHandler::is_numeric(const std::string& str) {
        // Check if all characters in the string are numeric
        return std::all_of(str.begin(), str.end(), [](unsigned char c) {
            return std::isdigit(c);
        });
    }

    std::string generate_append_msg(const std::string& content)
    {
        // Content must be alphabetic within an append message
        if (!MessageHandler::is_alphabetic(content))
        {
            throw MTPException("Can't send a non-alphabetic string to the server");
        }

        return std::string(Constants::APPEND_STRING_PREFIX + content + Constants::MESSAGE_DELIMITER);
    }

    std::string generate_request_chunk_msg(const std::string& content)
    {
        // Content must be numeric within a request-chunk message
        if (!MessageHandler::is_numeric(content))
        {
            throw MTPException("Can't send a non-numeric-size chunk request to the server");
        }

        return std::string(Constants::REQUEST_CHUNK_PREFIX + content + Constants::MESSAGE_DELIMITER);
    }

    std::string generate_response_chunk_msg(const std::string& content)
    {
        // Content must be alphabetic when extracting a saved chunk
        if (!MessageHandler::is_alphabetic(content))
        {
            throw MTPException("Can't send a non-alphabetic chunk response to the client");
        }

        return std::string(Constants::RESPONSE_CHUNK_PREFIX + content + Constants::MESSAGE_DELIMITER);
    }

    std::string MessageHandler::generate_message(Types type, const std::string& content)
    {
        if (content.empty())
        {
            throw MTPException("Can't generate a message given an empty string");
        }

        // Can be optimised using a map between Types and a function that receives the content
        switch(type)
        {
            case Types::APPEND_STRING:
            {
                return generate_append_msg(content);
            }
            break;
        
            case Types::REQUEST_CHUNK:
            {
                return generate_request_chunk_msg(content);
            }
            break;

            case Types::RESPONSE_CHUNK:
            {
                return generate_response_chunk_msg(content);
            }
            break;

            default:
                throw MTPException("Can't generate a message from an unsupported MTP type");
        }

        // Impossible execution path..
        throw MTPException("Reached impossible code");
    }

    Types MessageHandler::get_message_type(const std::string& msg)
    {
        if (msg.size() < (Constants::PREFIX_LENGTH + Constants::MINIMUM_CONTENT_LENGTH + Constants::SUFFIX_LENGTH))
        {
            throw MTPException("Can't retrieve the type of an illegal message");
        }

        std::string type_byte(msg.substr(0, Constants::PREFIX_LENGTH));

        // This, too, can be optimized using mappings that are known at compile time
        if (type_byte == Constants::APPEND_STRING_PREFIX)
        {
            return Types::APPEND_STRING;
        }
        else if (type_byte == Constants::REQUEST_CHUNK_PREFIX)
        {
            return Types::REQUEST_CHUNK;
        }
        else if (type_byte == Constants::RESPONSE_CHUNK_PREFIX)
        {
            return Types::RESPONSE_CHUNK;
        }
        else
        {
            throw MTPException("Can't retrieve the message type of an unsupported message");
        }
    }

    std::string_view MessageHandler::get_message_content(const std::string& msg)
    {
        if (msg.size() < (Constants::PREFIX_LENGTH + Constants::MINIMUM_CONTENT_LENGTH + Constants::SUFFIX_LENGTH))
        {
            throw MTPException("Can't retrieve the content of an illegal message");
        }

        return std::string_view(msg).substr(Constants::PREFIX_LENGTH, msg.size() - Constants::PREFIX_LENGTH - Constants::SUFFIX_LENGTH);
    }
}