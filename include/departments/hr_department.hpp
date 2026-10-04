#pragma once

#include "department.hpp"

class HrDepartment : public Department {
public:
    HrDepartment();

    std::string getDescription() const override;
};
