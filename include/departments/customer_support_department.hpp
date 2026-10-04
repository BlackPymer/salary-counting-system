#pragma once

#include "department.hpp"

class CustomerSupportDepartment : public Department {
public:
    CustomerSupportDepartment();

    std::string getDescription() const override;
};
