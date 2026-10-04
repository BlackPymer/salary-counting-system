#include "departments/administration_department.hpp"

AdministrationDepartment::AdministrationDepartment() : Department("Администрация") {}

std::string AdministrationDepartment::getDescription() const {
    return "Управление хозяйственной деятельностью офиса";
}
