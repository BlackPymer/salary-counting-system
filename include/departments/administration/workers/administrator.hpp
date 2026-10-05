#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/manager.hpp"

class Administrator : public Manager {
public:
    static constexpr double kOfficeAdminBonus = 0.05;
    static constexpr int kBonusTaskThreshold = 10;

    Administrator(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                  std::unique_ptr<AdvancePayment> advance, std::string officeBuilding);

    std::string getRole() const override;

    const std::string& getOfficeBuilding() const;
    int getTasksCompleted() const;
    void completeTask();

protected:
    double calculateRoleBonus() const override;

private:
    std::string officeBuilding_;
    int tasksCompleted_ = 0;
};
