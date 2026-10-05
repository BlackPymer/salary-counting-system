#pragma once

#include <string>

#include "accountant.hpp"
#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"

class PayrollAccountant : public Accountant {
public:
    PayrollAccountant(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
                      std::unique_ptr<AdvancePayment> advance, std::string certificationLevel,
                      int employeesUnderService);

    std::string getRole() const override;

    int getEmployeesUnderService() const;

    bool servesDepartment() const;

protected:
    double calculateRoleBonus() const override;

private:
    int employeesUnderService_ = 0;
};
