#pragma once

#include <string>

#include "worker.hpp"

class Manager : public Worker {
public:
    using Worker::Worker;

    std::string getRole() const override;
    virtual std::string generateReport() const;

    bool approveLeave(int employeeId);

protected:
    double calculateRoleBonus() const override;

private:
    double managementBonusRate_ = 0.20;
};
