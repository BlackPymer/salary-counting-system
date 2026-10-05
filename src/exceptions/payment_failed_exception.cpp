#include "exceptions/payment_failed_exception.hpp"

PaymentFailedException::PaymentFailedException(const std::string& message)
    : BaseException(message) {}

PaymentFailedException::PaymentFailedException(const char* message) : BaseException(message) {}
