#include "exceptions/invalid_contract_exception.hpp"

InvalidContractException::InvalidContractException(const std::string& message)
    : BaseException(message) {}

InvalidContractException::InvalidContractException(const char* message) : BaseException(message) {}

int InvalidContractException::getErrorCode() const {
    return kErrorCode;
}
