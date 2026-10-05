#pragma once

#include "base_exception.hpp"

class InvalidInputException : public BaseException {
public:
    static constexpr int kErrorCode = 4008;

    explicit InvalidInputException(const std::string& message);
    explicit InvalidInputException(const char* message);

    ~InvalidInputException() override = default;
    int getErrorCode() const;
};
