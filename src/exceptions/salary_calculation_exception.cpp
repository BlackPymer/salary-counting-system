#include "exceptions/salary_calculation_exception.hpp"

SalaryCalculationException::SalaryCalculationException(const std::string& message)
    : BaseException(message) {}

SalaryCalculationException::SalaryCalculationException(const char* message)
    : BaseException(message) {}
