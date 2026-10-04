#include "departments/research_n_developement/workers/research_scientist.hpp"

#include "exceptions/invalid_input_exception.hpp"

namespace {
constexpr double kBaseResearchRate = 0.14;
constexpr double kProjectBonus = 0.08;
constexpr double kPublicationBonus = 0.06;
constexpr int kProjectThreshold = 2;
constexpr int kPublicationThreshold = 1;
}  // namespace

ResearchScientist::ResearchScientist(int id, std::string fullName,
                                     std::unique_ptr<EmploymentContract> contract,
                                     std::unique_ptr<AdvancePayment> advance, std::string researchArea)
    : Specialist(id, std::move(fullName), std::move(contract), std::move(advance)),
      researchArea_(std::move(researchArea)) {
    if (researchArea_.empty()) {
        throw InvalidInputException("Область исследования обязательна");
    }
}

std::string ResearchScientist::getRole() const {
    return "ResearchScientist";
}

std::string ResearchScientist::getSpecialization() const {
    return "НИОКР: " + researchArea_;
}

const std::string& ResearchScientist::getResearchArea() const {
    return researchArea_;
}

int ResearchScientist::getProjectsCompleted() const {
    return projectsCompleted_;
}

int ResearchScientist::getPublicationsCount() const {
    return publicationsCount_;
}

void ResearchScientist::completeProject() {
    ++projectsCompleted_;
}

void ResearchScientist::publishPaper() {
    ++publicationsCount_;
}

bool ResearchScientist::hasPublicationRecord() const {
    return publicationsCount_ >= kPublicationThreshold;
}

double ResearchScientist::calculateRoleBonus() const {
    const double projectBonus =
        projectsCompleted_ >= kProjectThreshold ? kProjectBonus : 0.0;
    const double publicationBonus = hasPublicationRecord() ? kPublicationBonus : 0.0;
    return getBaseRate() * (kBaseResearchRate + projectBonus + publicationBonus);
}
