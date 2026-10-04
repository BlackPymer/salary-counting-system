#pragma once

#include <string>

#include "worker.hpp"

// Руководитель: надбавка за руководство, право утверждать отпуска.
class Manager : public Worker {
public:
    using Worker::Worker;

    std::string getRole() const override;
    virtual std::string generateReport() const;

    // Утверждает отсутствие сотрудника своего отдела.
    bool approveLeave(int employeeId);

protected:
    double calculateRoleBonus() const override;

private:
    double managementBonusRate_ = 0.20;
};
