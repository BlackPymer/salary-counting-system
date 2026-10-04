#include "departments/it_department.hpp"

ItDepartment::ItDepartment() : Department("IT-отдел") {}

std::string ItDepartment::getDescription() const {
    return "Разработка ПО, инфраструктура и техническая поддержка";
}
