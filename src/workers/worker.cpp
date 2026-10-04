#include "workers/worker.hpp"

#include "departments/department.hpp"
#include "exceptions/invalid_input_exception.hpp"
#include "exceptions/overtime_limit_exceeded_exception.hpp"

Worker::Worker(int id, std::string fullName, std::unique_ptr<EmploymentContract> contract,
               std::unique_ptr<AdvancePayment> advance)
    : id_(id),
      fullName_(std::move(fullName)),
      contract_(std::move(contract)),
      advance_(std::move(advance)) {}

int Worker::getId() const {
    return id_;
}

const std::string& Worker::getFullName() const {
    return fullName_;
}

const std::string& Worker::getPosition() const {
    return contract_->getPosition();
}

Date Worker::getHireDate() const {
    return contract_->getHireDate();
}

double Worker::getBaseRate() const {
    return contract_->getMonthlyRate();
}

double Worker::getEffectiveRate() const {
    return contract_->getEffectiveRateOn(today());
}

bool Worker::isOnProbation() const {
    return contract_->isOnProbationOn(today());
}

bool Worker::isActive() const {
    return contract_->isActiveOn(today());
}

Salary Worker::calculateSalary() const {
    const double base = getEffectiveRate();
    Salary salary(base + calculateRoleBonus(), 0.0, base + calculateRoleBonus());

    if (advance_ != nullptr) {
        salary.applyRepayment(advance_->applyDeduction(salary.getNet()));
    }
    return salary;
}

std::string Worker::getRole() const {
    return "Worker";
}

void Worker::addAbsence(std::unique_ptr<Absence> absence) {
    if (absence == nullptr) {
        throw InvalidInputException("Передано пустое отсутствие");
    }
    absences_.push_back(std::move(absence));
}

const std::vector<std::unique_ptr<Absence>>& Worker::getAbsences() const {
    return absences_;
}

int Worker::getTotalAbsentDays() const {
    int total = 0;
    for (const std::unique_ptr<Absence>& absence : absences_) {
        total += absence->getDays();
    }
    return total;
}

double Worker::getAveragePayRate() const {
    if (absences_.empty()) {
        return 1.0;
    }
    double total = 0.0;
    for (const std::unique_ptr<Absence>& absence : absences_) {
        total += absence->getPayRate();
    }
    return total / absences_.size();
}

void Worker::registerOvertime(double hours) {
    if (hours <= 0.0) {
        throw InvalidInputException("Количество сверхурочных часов должно быть положительным");
    }
    if (overtimeHours_ + hours > kOvertimeLimitHours) {
        throw OvertimeLimitExceededException(fullName_, overtimeHours_ + hours, kOvertimeLimitHours);
    }
    overtimeHours_ += hours;
}

double Worker::getOvertimeHours() const {
    return overtimeHours_;
}

void Worker::resetPeriod() {
    absences_.clear();
    overtimeHours_ = 0.0;
}

void Worker::setDepartment(Department* department) {
    department_ = department;
}

Department* Worker::getDepartment() const {
    return department_;
}

bool Worker::worksIn(const Department* department) const {
    return department_ == department;
}

const EmploymentContract& Worker::getContract() const {
    return *contract_;
}

EmploymentContract& Worker::getContract() {
    return *contract_;
}

AdvancePayment& Worker::getAdvance() {
    return *advance_;
}

const AdvancePayment& Worker::getAdvance() const {
    return *advance_;
}

double Worker::calculateRoleBonus() const {
    return 0.0;
}
