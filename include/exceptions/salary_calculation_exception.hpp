#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class SalaryCalculationException : public BaseException {
public:
    static constexpr int kErrorCode = 4011;

    explicit SalaryCalculationException(const std::string& message);
    explicit SalaryCalculationException(const char* message);
    int getErrorCode() const;
};
