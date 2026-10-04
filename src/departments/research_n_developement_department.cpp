#include "departments/research_n_developement_department.hpp"

ResearchNDevelopementDepartment::ResearchNDevelopementDepartment() : Department("НИОКР") {}

std::string ResearchNDevelopementDepartment::getDescription() const {
    return "Исследования, разработки, новые продукты";
}
