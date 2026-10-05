#pragma once

#include <string>

#include "base_exception.hpp"

class EmployeeNotFoundException : public BaseException {
public:
    static constexpr int kErrorCode = 4004;
    int getErrorCode() const;

    EmployeeNotFoundException(int employeeId, const std::string& departmentName);

    int getEmployeeId() const;
    const std::string& getDepartmentName() const;

    ~EmployeeNotFoundException() override = default;

private:
    int employeeId_ = 0;
    std::string departmentName_;
};
