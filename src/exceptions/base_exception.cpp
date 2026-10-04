#include "exceptions/base_exception.hpp"

BaseException::BaseException(const std::string& message) : std::runtime_error(message) {}

BaseException::BaseException(const char* message) : std::runtime_error(message) {}
