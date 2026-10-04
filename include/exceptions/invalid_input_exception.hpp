#pragma once

#include "base_exception.hpp"

class InvalidInputException : public BaseException {
public:
    explicit InvalidInputException(const std::string& message);
    explicit InvalidInputException(const char* message);

    ~InvalidInputException() override = default;
};
