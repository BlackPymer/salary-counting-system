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

// Тип сотрудника. Определяет, кого именно создаст фабрика при найме.
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

// Базовый сотрудник. Владеет контрактом и авансом, ссылается на отдел.
// Отдел, в свою очередь, владеет сотрудником.
class Worker {
public:
    // Норматив сверхурочных за расчётный период.
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

    // Базовая зарплата с учётом испытательного срока и зачёта аванса.
    // Налог на этом шаге не удерживается — придёт вместе с payroll.
    virtual Salary calculateSalary() const;

    virtual std::string getRole() const;

    // Факты, сообщённые симуляцией: сколько дней отсутствовал и почему.
    void addAbsence(std::unique_ptr<Absence> absence);
    const std::vector<std::unique_ptr<Absence>>& getAbsences() const;
    int getTotalAbsentDays() const;

    // Средняя ставка оплаты по всем отсутствиям за период.
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
    // Надбавка за должность; переопределяется в Specialist и Manager.
    virtual double calculateRoleBonus() const;

private:
    int id_ = 0;
    std::string fullName_;
    std::unique_ptr<EmploymentContract> contract_;
    std::unique_ptr<AdvancePayment> advance_;
    Department* department_ = nullptr;
    std::vector<std::unique_ptr<Absence>> absences_;
    double overtimeHours_ = 0.0;
};
