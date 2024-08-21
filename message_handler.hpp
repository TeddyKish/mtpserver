#pragma once

#include <string>
#include <mutex>

namespace MTP
{
	namespace Constants {
		constexpr char MESSAGE_DELIMITER = ','; // Crucial for determining the end of each "packet"
		constexpr uint8_t SUFFIX_LENGTH = 1;
		constexpr char UDS_PATH[] = "/tmp/abc";
	}

	// Contains the different types of MTP messages
	enum class Types : uint8_t
	{
		APPEND_STRING, // Client->Server
		REQUEST_CHUNK, // Client->Server
		RESPONSE_CHUNK // Server->Client
	};

	class MessageHandler
	{
	public:
		/**
		 * Generates an MTP message according to the parameters.
		 * @param type: The type of the message to generate
		 * @param content: The content to place within the message
		 * @return The generated message.
		 */
		static std::string generate_message(Types type, const std::string& content);

		/**
		 * @param msg The msg to get the type from.
		 * @return Returns <msg>'s MTP message type.
		 */
		static Types get_message_type(const std::string& msg);

		/**
		 *
		 * @param msg An MTP message.
		 * @return Returns a view to the content within the MTP message <msg>.
		 */
		static std::string_view get_message_content(const std::string& msg);

		/**
		 * @param str A string.
		 * @return Whether <str> is composed exclusively of alphabetic characters.
		 */
		static bool is_alphabetic(const std::string& str);

		/**
		 * @param str A string.
		 * @return Whether <str> is composed exclusively of numeric characters.
		 */
		static bool is_numeric(const std::string& str);
	};
}