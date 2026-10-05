#include "payroll/calculation_context.hpp"

#include "exceptions/invalid_input_exception.hpp"

CalculationContext::CalculationContext(const Worker* worker, const PayrollPeriod& period)
    : worker_(worker), period_(period), calculationDate_(period.getLastDay()) {
    if (worker_ == nullptr) {
        throw InvalidInputException("Работник не может быть пустым в контексте расчёта");
    }
}

CalculationContext::CalculationContext(const Worker* worker, const PayrollPeriod& period,
                                       const Date& calculationDate)
    : worker_(worker), period_(period), calculationDate_(calculationDate) {
    if (worker_ == nullptr) {
        throw InvalidInputException("Работник не может быть пустым в контексте расчёта");
    }
}

const Worker& CalculationContext::getWorker() const {
    return *worker_;
}

const PayrollPeriod& CalculationContext::getPeriod() const {
    return period_;
}

const Date& CalculationContext::getCalculationDate() const {
    return calculationDate_;
}

double CalculationContext::getBaseRate() const {
    return worker_->getBaseRate();
}

double CalculationContext::getEffectiveRate() const {
    return worker_->getEffectiveRate();
}

bool CalculationContext::isOnProbation() const {
    return worker_->isOnProbation();
}

bool CalculationContext::isActive() const {
    return worker_->isActive();
}

double CalculationContext::getOvertimeHours() const {
    return worker_->getOvertimeHours();
}

int CalculationContext::getTotalAbsentDays() const {
    return worker_->getTotalAbsentDays();
}

double CalculationContext::getAveragePayRate() const {
    return worker_->getAveragePayRate();
}

const std::vector<std::unique_ptr<Absence>>& CalculationContext::getAbsences() const {
    return worker_->getAbsences();
}

const AdvancePayment& CalculationContext::getAdvance() const {
    return worker_->getAdvance();
}

const EmploymentContract& CalculationContext::getContract() const {
    return worker_->getContract();
}
