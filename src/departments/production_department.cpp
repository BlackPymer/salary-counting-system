#include "departments/production_department.hpp"

ProductionDepartment::ProductionDepartment() : Department("Производство") {}

std::string ProductionDepartment::getDescription() const {
    return "Выпуск продукции, техническое обслуживание оборудования";
}
