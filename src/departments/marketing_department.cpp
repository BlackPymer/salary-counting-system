#include "departments/marketing_department.hpp"

MarketingDepartment::MarketingDepartment() : Department("Маркетинг") {}

std::string MarketingDepartment::getDescription() const {
    return "Продвижение, реклама, анализ рынка";
}
