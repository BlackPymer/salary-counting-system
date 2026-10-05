#include "exceptions/file_io_exception.hpp"

FileIoException::FileIoException(const std::string& message) : BaseException(message) {}

FileIoException::FileIoException(const char* message) : BaseException(message) {}

int FileIoException::getErrorCode() const {
    return kErrorCode;
}
