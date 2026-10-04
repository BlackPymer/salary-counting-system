#pragma once

#include <string>

#include "base_exception.hpp"

class InvalidContractException : public BaseException {
public:
    explicit InvalidContractException(const std::string& message);
    explicit InvalidContractException(const char* message);

    ~InvalidContractException() override = default;
};
