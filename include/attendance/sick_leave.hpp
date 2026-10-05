#pragma once

#include <memory>
#include <string>

#include "absence.hpp"

class SickLeave : public Absence {
public:
    SickLeave(int days, int seniorityYears);
    ~SickLeave() override = default;

    static std::unique_ptr<SickLeave> forDays(int days, int seniorityYears);

    int getSeniorityYears() const;
    double getPayRate() const override;
    std::string getTypeName() const override;

private:
    int seniorityYears_ = 0;
};
