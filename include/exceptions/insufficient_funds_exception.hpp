#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class InsufficientFundsException : public BaseException {
public:
    explicit InsufficientFundsException(const std::string& message);
    explicit InsufficientFundsException(const char* message);
};
