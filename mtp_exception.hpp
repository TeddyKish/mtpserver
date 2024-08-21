#include <exception>
#include <string>

namespace MTP {
    /**
     * A helper exception class that wraps every exception within this module.
     */
    class MTPException : public std::exception {
    public:
        explicit MTPException(const std::string& message) : message_("MTP Error occured: " + message) {}

        const char* what() const noexcept override {
            return message_.c_str();
        }

    private:
        std::string message_;
    };
}