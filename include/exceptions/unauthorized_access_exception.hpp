#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class UnauthorizedAccessException : public BaseException {
public:
    static constexpr int kErrorCode = 4013;

    explicit UnauthorizedAccessException(const std::string& message);
    explicit UnauthorizedAccessException(const char* message);
    int getErrorCode() const;
};
