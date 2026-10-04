#include "company.hpp"

#include <algorithm>
#include <sstream>

#include "core/date.hpp"
#include "core/worker_factory.hpp"
#include "departments/hr/workers/recruiter.hpp"
#include "departments/hr_department.hpp"
#include "exceptions/department_not_found_exception.hpp"
#include "exceptions/employee_not_found_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"

Company::Company(std::string name) : name_(std::move(name)) {
    if (name_.empty()) {
        throw InvalidInputException("Название компании не может быть пустым");
    }
}

void Company::addDepartment(std::unique_ptr<Department> department) {
    if (department == nullptr) {
        throw InvalidInputException("Передан пустой отдел");
    }
    if (findDepartment(department->getName()) != nullptr) {
        throw InvalidInputException("Отдел '" + department->getName() + "' уже существует");
    }
    departments_.push_back(std::move(department));
}

Department* Company::findDepartment(const std::string& name) const {
    const auto it = std::find_if(departments_.begin(), departments_.end(),
                                 [&name](const std::unique_ptr<Department>& department) {
                                     return department->getName() == name;
                                 });
    return it == departments_.end() ? nullptr : it->get();
}

std::size_t Company::getDepartmentsCount() const {
    return departments_.size();
}

Worker* Company::appointRecruiter(const std::string& fullName, double monthlyRate) {
    if (fullName.empty()) {
        throw InvalidInputException("Имя сотрудника не может быть пустым");
    }
    Department* hrDepartment = findDepartment(HrDepartment::kName);
    if (hrDepartment == nullptr) {
        throw DepartmentNotFoundException(HrDepartment::kName);
    }

    // Условия определяет сама компания — рекрутер ещё не существует,
    // готовить их некому.
    auto contract = std::make_unique<EmploymentContract>(
        Recruiter::contractNumberFor(nextWorkerId_), WorkerFactory::positionFor(WorkerType::Recruiter),
        today(), monthlyRate);
    auto advance = std::make_unique<AdvancePayment>(monthlyRate * Recruiter::kMaxAdvanceRate, today());

    std::unique_ptr<Worker> worker = WorkerFactory::create(
        nextWorkerId_, fullName, WorkerType::Recruiter, std::move(contract), std::move(advance));

    const int workerId = nextWorkerId_;
    hrDepartment->addWorker(std::move(worker));
    ++nextWorkerId_;

    return hrDepartment->findWorker(workerId);
}

Worker* Company::hireWorker(const std::string& departmentName, const std::string& fullName,
                            WorkerType type, double monthlyRate) {
    if (fullName.empty()) {
        throw InvalidInputException("Имя сотрудника не может быть пустым");
    }

    Department* department = findDepartment(departmentName);
    if (department == nullptr) {
        throw DepartmentNotFoundException(departmentName);
    }

    Recruiter* recruiter = findRecruiter();
    if (recruiter == nullptr) {
        throw EmployeeNotFoundException(0, "Отдел кадров (нет рекрутера для найма)");
    }

    const std::string position = WorkerFactory::positionFor(type);

    // Условия найма готовит отдел кадров: контракт с испытательным сроком
    // и аванс в пределах разрешённой доли ставки.
    std::unique_ptr<EmploymentContract> contract =
        recruiter->createContract(nextWorkerId_, position, monthlyRate);
    std::unique_ptr<AdvancePayment> advance =
        recruiter->createAdvance(monthlyRate * Recruiter::kMaxAdvanceRate);
    std::unique_ptr<Worker> worker =
        WorkerFactory::create(nextWorkerId_, fullName, type, std::move(contract), std::move(advance));

    const int workerId = nextWorkerId_;
    recruiter->registerCandidate();
    department->addWorker(std::move(worker));
    recruiter->closeVacancy();
    ++nextWorkerId_;

    return department->findWorker(workerId);
}

std::unique_ptr<Worker> Company::terminateWorker(const std::string& departmentName, int workerId) {
    Department* department = findDepartment(departmentName);
    if (department == nullptr) {
        throw DepartmentNotFoundException(departmentName);
    }
    return department->removeWorker(workerId);
}

double Company::calculateCompanyPayroll() const {
    double total = 0.0;
    for (const std::unique_ptr<Department>& department : departments_) {
        total += department->calculatePayroll();
    }
    return total;
}

int Company::getTotalWorkersCount() const {
    int total = 0;
    for (const std::unique_ptr<Department>& department : departments_) {
        total += static_cast<int>(department->getWorkersCount());
    }
    return total;
}

std::string Company::generateReport() const {
    std::ostringstream report;
    report << "Компания '" << name_ << "', отделов: " << departments_.size()
           << ", сотрудников: " << getTotalWorkersCount() << '\n';
    for (const std::unique_ptr<Department>& department : departments_) {
        report << "  - " << department->getDescription() << '\n';
    }
    report << "ФОТ: " << calculateCompanyPayroll() << '\n';
    return report.str();
}

Recruiter* Company::findRecruiter() const {
    for (const std::unique_ptr<Department>& department : departments_) {
        for (Worker* worker : department->getWorkers()) {
            if (auto* recruiter = dynamic_cast<Recruiter*>(worker)) {
                return recruiter;
            }
        }
    }
    return nullptr;
}

