#include "departments/customer_support_department.hpp"

CustomerSupportDepartment::CustomerSupportDepartment()
    : Department("Клиентская поддержка", "Работа с обращениями клиентов, техническая помощь",
                 kMaxHeadcount, kMonthlyBudget) {}
