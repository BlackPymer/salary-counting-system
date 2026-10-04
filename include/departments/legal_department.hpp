#pragma once

#include "department.hpp"

class LegalDepartment : public Department {
public:
    LegalDepartment();

    std::string getDescription() const override;
};
