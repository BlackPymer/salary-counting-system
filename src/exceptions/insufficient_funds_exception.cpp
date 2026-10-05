#include "exceptions/insufficient_funds_exception.hpp"

InsufficientFundsException::InsufficientFundsException(const std::string& message)
    : BaseException(message) {}

InsufficientFundsException::InsufficientFundsException(const char* message)
    : BaseException(message) {}
