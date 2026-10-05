#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class FileIoException : public BaseException {
public:
    explicit FileIoException(const std::string& message);
    explicit FileIoException(const char* message);
};
