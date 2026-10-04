#pragma once

#include "department.hpp"

class ResearchNDevelopementDepartment : public Department {
public:
    ResearchNDevelopementDepartment();

    std::string getDescription() const override;
};
