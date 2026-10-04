#pragma once

#include <chrono>
#include <memory>
#include <string>

#include "core/date.hpp"
#include "financial_objects/advance_payment.hpp"
#include "financial_objects/employment_contract.hpp"

// year_month_day не поддерживает арифметику с днями и месяцами напрямую,
// поэтому сдвиг дат в тестах идёт через sys_days.
inline Date daysAfter(const Date& date, int days) {
    return Date{std::chrono::sys_days{date} + std::chrono::days{days}};
}

inline Date daysBefore(const Date& date, int days) {
    return daysAfter(date, -days);
}

// Общие заготовки условий найма для тестов.
inline std::unique_ptr<EmploymentContract> makeContract(double rate = 100000.0,
                                                        const std::string& position = "Должность") {
    return std::make_unique<EmploymentContract>("T-TEST", position, today(), rate);
}

inline std::unique_ptr<AdvancePayment> makeAdvance(double amount = 1000.0) {
    return std::make_unique<AdvancePayment>(amount, today());
}
