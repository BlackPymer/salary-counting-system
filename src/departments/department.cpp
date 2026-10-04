#include "departments/department.hpp"

#include <algorithm>

#include "exceptions/duplicate_employee_exception.hpp"
#include "exceptions/employee_not_found_exception.hpp"
#include "exceptions/invalid_input_exception.hpp"

Department::Department(std::string name) : name_(std::move(name)) {
    if (name_.empty()) {
        throw InvalidInputException("Название отдела не может быть пустым");
    }
}

void Department::addWorker(std::unique_ptr<Worker> worker) {
    if (worker == nullptr) {
        throw InvalidInputException("Передан пустой сотрудник");
    }
    if (hasWorker(worker->getId())) {
        throw DuplicateEmployeeException(worker->getId(), worker->getFullName());
    }
    worker->setDepartment(this);
    workers_.push_back(std::move(worker));
}

std::unique_ptr<Worker> Department::removeWorker(int workerId) {
    const auto it = std::find_if(workers_.begin(), workers_.end(),
                                 [workerId](const std::unique_ptr<Worker>& worker) {
                                     return worker->getId() == workerId;
                                 });
    if (it == workers_.end()) {
        throw EmployeeNotFoundException(workerId, name_);
    }
    std::unique_ptr<Worker> removed = std::move(*it);
    workers_.erase(it);
    removed->setDepartment(nullptr);
    return removed;
}

Worker* Department::findWorker(int workerId) const {
    const auto it = std::find_if(workers_.begin(), workers_.end(),
                                 [workerId](const std::unique_ptr<Worker>& worker) {
                                     return worker->getId() == workerId;
                                 });
    return it == workers_.end() ? nullptr : it->get();
}

bool Department::hasWorker(int workerId) const {
    return findWorker(workerId) != nullptr;
}

std::vector<Worker*> Department::getWorkers() const {
    std::vector<Worker*> result;
    result.reserve(workers_.size());
    for (const std::unique_ptr<Worker>& worker : workers_) {
        result.push_back(worker.get());
    }
    return result;
}

std::size_t Department::getWorkersCount() const {
    return workers_.size();
}

double Department::calculatePayroll() const {
    double total = 0.0;
    for (const std::unique_ptr<Worker>& worker : workers_) {
        total += worker->calculateSalary().getNet();
    }
    return total;
}

const std::string& Department::getName() const {
    return name_;
}

std::string Department::getDescription() const {
    return "Отдел '" + name_ + "', сотрудников: " + std::to_string(workers_.size());
}
