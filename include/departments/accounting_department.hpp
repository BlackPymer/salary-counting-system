#pragma once

#include "department.hpp"

class AccountingDepartment : public Department {
public:
    AccountingDepartment();

    std::string getDescription() const override;
};
