#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class ContractExpiredException : public BaseException {
public:
    explicit ContractExpiredException(const std::string& message);
    explicit ContractExpiredException(const char* message);
};
