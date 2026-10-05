#pragma once

#include <string>

#include "base_exception.hpp"

class DuplicateEmployeeException : public BaseException {
public:
    static constexpr int kErrorCode = 4003;
    int getErrorCode() const;

    DuplicateEmployeeException(int employeeId, const std::string& fullName);

    int getEmployeeId() const;
    const std::string& getFullName() const;

    ~DuplicateEmployeeException() override = default;

private:
    int employeeId_ = 0;
    std::string fullName_;
};
