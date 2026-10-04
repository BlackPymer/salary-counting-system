#pragma once

#include <stdexcept>
#include <string>

class BaseException : public std::runtime_error {
public:
    explicit BaseException(const std::string& message);
    explicit BaseException(const char* message);

    ~BaseException() override = default;
};
