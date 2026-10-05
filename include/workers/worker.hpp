#pragma once

#include <memory>
#include <string>
#include <vector>

#include "attendance/absence.hpp"
#include "core/date.hpp"
#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"
#include "financial_objects/salary.hpp"

class Department;

enum class WorkerType {
    Accountant,
    PayrollAccountant,
    Administrator,
    CustomerSupportAgent,
    Recruiter,
    SoftwareDeveloper,
    SystemAdministrator,
    Lawyer,
    Marketer,
    Technician,
    ResearchScientist,
    SecurityGuard,
};

class Worker {
public:
    static constexpr double kOvertimeLimitHours = 80.0;

    Worker() = default;
    Worker(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
           std::unique_ptr<AdvancePayment> advance);
    virtual ~Worker() = default;

    Worker(const Worker&) = delete;
    Worker& operator=(const Worker&) = delete;
    Worker(Worker&&) = delete;
    Worker& operator=(Worker&&) = delete;

    int getId() const;
    const std::string& getFullName() const;
    const std::string& getPosition() const;
    Date getHireDate() const;

    double getBaseRate() const;
    double getEffectiveRate() const;

    bool isOnProbation() const;
    bool isActive() const;

    virtual Salary calculateSalary() const;

    double repayAdvance(double availableAmount);

    void resetAdvance();

    virtual std::string getRole() const;

    void addAbsence(std::unique_ptr<Absence> absence);
    const std::vector<std::unique_ptr<Absence>>& getAbsences() const;
    int getTotalAbsentDays() const;

    double getAveragePayRate() const;

    void registerOvertime(double hours);
    double getOvertimeHours() const;
    void resetPeriod();

    void setDepartment(Department* department);
    Department* getDepartment() const;
    bool worksIn(const Department* department) const;

    const EmploymentContract& getContract() const;
    EmploymentContract& getContract();
    AdvancePayment& getAdvance();
    const AdvancePayment& getAdvance() const;

protected:
    virtual double calculateRoleBonus() const;

private:
    double computeAbsenceAdjustment() const;
    double computeOvertimePay() const;
    double computeBonus() const;
    double computeTax(double gross) const;

    int id_ = 0;
    std::string fullName_;
    std::unique_ptr<EmploymentContract> contract_;
    std::unique_ptr<AdvancePayment> advance_;
    Department* department_ = nullptr;
    std::vector<std::unique_ptr<Absence>> absences_;
    double overtimeHours_ = 0.0;
};
