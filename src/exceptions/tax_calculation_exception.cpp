#include "exceptions/tax_calculation_exception.hpp"

TaxCalculationException::TaxCalculationException(const std::string& message)
    : BaseException(message) {}

TaxCalculationException::TaxCalculationException(const char* message) : BaseException(message) {}

int TaxCalculationException::getErrorCode() const {
    return kErrorCode;
}
