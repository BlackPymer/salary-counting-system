#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class TaxCalculationException : public BaseException {
public:
    explicit TaxCalculationException(const std::string& message);
    explicit TaxCalculationException(const char* message);
};
