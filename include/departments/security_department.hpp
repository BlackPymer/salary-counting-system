#pragma once

#include "department.hpp"

class SecurityDepartment : public Department {
public:
    SecurityDepartment();

    std::string getDescription() const override;
};
