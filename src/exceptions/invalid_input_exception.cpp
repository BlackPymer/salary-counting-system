#include "exceptions/invalid_input_exception.hpp"

InvalidInputException::InvalidInputException(const std::string& message) : BaseException(message) {}

InvalidInputException::InvalidInputException(const char* message) : BaseException(message) {}
