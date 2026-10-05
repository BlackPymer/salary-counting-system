#pragma once

#include <string>

#include "worker.hpp"

class Specialist : public Worker {
public:
    using Worker::Worker;

    std::string getRole() const override;
    virtual std::string getSpecialization() const;
    double getSpecializationBonusRate() const;

protected:
    double calculateRoleBonus() const override;

private:
    double specializationBonusRate_ = 0.10;
};
