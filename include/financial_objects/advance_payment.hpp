#pragma once

#include "core/date.hpp"

// Аванс, выдаваемый при найме. Долга не возникает: списание зарплатой
// ограничено остатком аванса и суммой на руки.
class AdvancePayment {
public:
    AdvancePayment() = default;
    AdvancePayment(double amount, Date issueDate);

    double getAmount() const;
    double getRepaidAmount() const;
    double getRemaining() const;
    bool isFullyRepaid() const;
    Date getIssueDate() const;

    // Списывает не больше остатка аванса и не больше доступной суммы.
    // Возвращает фактически списанную сумму.
    double applyDeduction(double availableAmount);

private:
    double amount_ = 0.0;
    double repaidAmount_ = 0.0;
    Date issueDate_;
};
