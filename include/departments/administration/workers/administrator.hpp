#pragma once

#include <string>

#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "workers/manager.hpp"

// Администратор офиса: руководитель хозяйственной службы.
class Administrator : public Manager {
public:
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
