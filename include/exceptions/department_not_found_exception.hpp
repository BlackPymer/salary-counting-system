#pragma once

#include <string>

#include "base_exception.hpp"

class DepartmentNotFoundException : public BaseException {
public:
    explicit DepartmentNotFoundException(const std::string& departmentName);

    const std::string& getDepartmentName() const;

    ~DepartmentNotFoundException() override = default;

private:
    std::string departmentName_;
};
