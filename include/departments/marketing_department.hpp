#pragma once

#include "department.hpp"

class MarketingDepartment : public Department {
public:
    MarketingDepartment();

    std::string getDescription() const override;
};
