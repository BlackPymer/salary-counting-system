#pragma once

#include <chrono>

using Date = std::chrono::year_month_day;

int daysBetween(const Date& from, const Date& to);
bool isEarlier(const Date& lhs, const Date& rhs);
bool isSameOrEarlier(const Date& lhs, const Date& rhs);
Date today();
