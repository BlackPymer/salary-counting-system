#include "departments/legal_department.hpp"

LegalDepartment::LegalDepartment()
    : Department("Юридический отдел", "Правовое сопровождение, договоры, комплаенс", kMaxHeadcount,
                 kMonthlyBudget) {}
