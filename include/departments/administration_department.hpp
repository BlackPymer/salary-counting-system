#pragma once

#include "department.hpp"

class AdministrationDepartment : public Department {
public:
    AdministrationDepartment();

    std::string getDescription() const override;
};
