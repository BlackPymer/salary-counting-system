#include "departments/hr_department.hpp"

HrDepartment::HrDepartment() : Department(kName) {}

std::string HrDepartment::getDescription() const {
    return "Найм, адаптация и кадровое делопроизводство";
}
