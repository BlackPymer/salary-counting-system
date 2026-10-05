#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class PaymentFailedException : public BaseException {
public:
    explicit PaymentFailedException(const std::string& message);
    explicit PaymentFailedException(const char* message);
};
