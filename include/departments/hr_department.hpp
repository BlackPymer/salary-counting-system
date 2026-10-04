#pragma once

#include "department.hpp"

class HrDepartment : public Department {
public:
    static constexpr const char* kName = "Отдел кадров";

    HrDepartment();

    std::string getDescription() const override;
};
