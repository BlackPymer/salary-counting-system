#include "departments/accounting/workers/payroll_accountant.hpp"

#include "exceptions/invalid_input_exception.hpp"

namespace {
constexpr double kPayrollBonus = 0.10;
constexpr int kMaxEmployeesPerPayrollAccountant = 50;
}  // namespace

PayrollAccountant::PayrollAccountant(int id, std::string fullName,
                                     std::unique_ptr<EmploymentContract> contract,
                                     std::unique_ptr<AdvancePayment> advance,
                                     std::string certificationLevel, int employeesUnderService)
    : Accountant(id, std::move(fullName), std::move(contract), std::move(advance),
                 std::move(certificationLevel)),
      employeesUnderService_(employeesUnderService) {
    if (employeesUnderService_ < 0) {
        throw InvalidInputException("Количество обслуживаемых сотрудников не может быть отрицательным");
    }
}

std::string PayrollAccountant::getRole() const {
    return "PayrollAccountant";
}

int PayrollAccountant::getEmployeesUnderService() const {
    return employeesUnderService_;
}

bool PayrollAccountant::servesDepartment() const {
    return employeesUnderService_ <= kMaxEmployeesPerPayrollAccountant;
}

double PayrollAccountant::calculateRoleBonus() const {
    return Accountant::calculateRoleBonus() + getBaseRate() * kPayrollBonus;
}
