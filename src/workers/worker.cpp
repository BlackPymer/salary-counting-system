#include "workers/worker.hpp"

#include "departments/department.hpp"

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
        salary.applyDeduction(advance_->applyDeduction(salary.getNet()));
    }
    return salary;
}

std::string Worker::getRole() const {
    return "Worker";
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
