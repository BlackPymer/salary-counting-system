#include "departments/hr_department.hpp"

HrDepartment::HrDepartment() : Department("Отдел кадров") {}

std::string HrDepartment::getDescription() const {
    return "Найм, адаптация и кадровое делопроизводство";
}
