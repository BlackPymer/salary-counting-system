#pragma once

#include <string>
#include "exceptions/base_exception.hpp"

class FileIoException : public BaseException {
public:
    static constexpr int kErrorCode = 4005;

    explicit FileIoException(const std::string& message);
    explicit FileIoException(const char* message);
    int getErrorCode() const;
};
