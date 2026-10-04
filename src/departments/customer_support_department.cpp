#include "departments/customer_support_department.hpp"

CustomerSupportDepartment::CustomerSupportDepartment() : Department("Клиентская поддержка") {}

std::string CustomerSupportDepartment::getDescription() const {
    return "Работа с обращениями клиентов, техническая помощь";
}
