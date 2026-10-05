#include "financial_objects/advance_payment.hpp"

#include <algorithm>

#include "exceptions/invalid_input_exception.hpp"

AdvancePayment::AdvancePayment(double amount, Date issueDate)
    : amount_(amount), issueDate_(issueDate) {
    if (amount <= 0.0) {
        throw InvalidInputException("Сумма аванса должна быть положительной");
    }
    repaymentDue_ = Date{std::chrono::sys_days{issueDate} + std::chrono::days{kRepaymentTermDays}};
}

double AdvancePayment::getAmount() const {
    return amount_;
}

double AdvancePayment::getRepaidAmount() const {
    return repaidAmount_;
}

double AdvancePayment::getRemaining() const {
    return amount_ - repaidAmount_;
}

bool AdvancePayment::isFullyRepaid() const {
    return getRemaining() <= 0.0;
}

Date AdvancePayment::getIssueDate() const {
    return issueDate_;
}

Date AdvancePayment::getRepaymentDue() const {
    return repaymentDue_;
}

double AdvancePayment::applyDeduction(double availableAmount) {
    const double deductible = std::min(std::max(0.0, availableAmount), getRemaining());
    repaidAmount_ += deductible;
    return deductible;
}
