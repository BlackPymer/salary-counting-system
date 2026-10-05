#include "departments/production_department.hpp"

ProductionDepartment::ProductionDepartment()
    : Department("Производство", "Выпуск продукции, техническое обслуживание оборудования",
                 kMaxHeadcount, kMonthlyBudget) {}
