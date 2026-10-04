#include "core/date.hpp"

int daysBetween(const Date& from, const Date& to) {
    return static_cast<int>((std::chrono::sys_days{to} - std::chrono::sys_days{from}).count());
}

bool isEarlier(const Date& lhs, const Date& rhs) {
    return std::chrono::sys_days{lhs} < std::chrono::sys_days{rhs};
}

bool isSameOrEarlier(const Date& lhs, const Date& rhs) {
    return std::chrono::sys_days{lhs} <= std::chrono::sys_days{rhs};
}

Date today() {
    const auto now = std::chrono::floor<std::chrono::days>(std::chrono::system_clock::now());
    return Date{std::chrono::year_month_day{now}};
}
