#pragma once

#include <string>

#include "base_exception.hpp"

class InvalidContractException : public BaseException {
public:
    static constexpr int kErrorCode = 4007;

    explicit InvalidContractException(const std::string& message);
    explicit InvalidContractException(const char* message);

    ~InvalidContractException() override = default;
    int getErrorCode() const;
};
