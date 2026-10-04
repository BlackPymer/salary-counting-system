#include "departments/legal_department.hpp"

LegalDepartment::LegalDepartment() : Department("Юридический отдел") {}

std::string LegalDepartment::getDescription() const {
    return "Правовое сопровождение, договоры, комплаенс";
}
