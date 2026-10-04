#pragma once

#include "department.hpp"

class ProductionDepartment : public Department {
public:
    ProductionDepartment();

    std::string getDescription() const override;
};
