#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class SalaryCalculationException : public BaseException {
public:
    explicit SalaryCalculationException(const std::string& message);
    explicit SalaryCalculationException(const char* message);
};
