#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class ContractExpiredException : public BaseException {
public:
    static constexpr int kErrorCode = 4001;

    explicit ContractExpiredException(const std::string& message);
    explicit ContractExpiredException(const char* message);
    int getErrorCode() const;
};
