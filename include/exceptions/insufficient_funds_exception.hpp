#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class InsufficientFundsException : public BaseException {
public:
    static constexpr int kErrorCode = 4006;

    explicit InsufficientFundsException(const std::string& message);
    explicit InsufficientFundsException(const char* message);
    int getErrorCode() const;
};
