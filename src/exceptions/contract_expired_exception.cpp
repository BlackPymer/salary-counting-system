#include "exceptions/contract_expired_exception.hpp"

ContractExpiredException::ContractExpiredException(const std::string& message)
    : BaseException(message) {}

ContractExpiredException::ContractExpiredException(const char* message) : BaseException(message) {}
