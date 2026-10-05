#include "exceptions/unauthorized_access_exception.hpp"

UnauthorizedAccessException::UnauthorizedAccessException(const std::string& message)
    : BaseException(message) {}

UnauthorizedAccessException::UnauthorizedAccessException(const char* message)
    : BaseException(message) {}

int UnauthorizedAccessException::getErrorCode() const {
    return kErrorCode;
}
