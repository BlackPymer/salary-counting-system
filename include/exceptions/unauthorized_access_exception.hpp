#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class UnauthorizedAccessException : public BaseException {
public:
    explicit UnauthorizedAccessException(const std::string& message);
    explicit UnauthorizedAccessException(const char* message);
};
