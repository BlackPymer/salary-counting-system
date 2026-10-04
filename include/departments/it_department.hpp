#pragma once

#include "department.hpp"

class ItDepartment : public Department {
public:
    ItDepartment();

    std::string getDescription() const override;
};
