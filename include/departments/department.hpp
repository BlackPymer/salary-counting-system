#pragma once

#include <memory>
#include <string>
#include <vector>

#include "workers/worker.hpp"

class Department {
public:
    static constexpr int kDefaultHeadcountLimit = 100;
    static constexpr double kDefaultMonthlyBudget = 1000000.0;

    Department() = default;
    explicit Department(std::string name);
    Department(std::string name, std::string description, int headcountLimit, double monthlyBudget);
    virtual ~Department() = default;

    Department(const Department&) = delete;
    Department& operator=(const Department&) = delete;
    Department(Department&&) = delete;
    Department& operator=(Department&&) = delete;

    void addWorker(std::unique_ptr<Worker> worker);

    std::unique_ptr<Worker> removeWorker(int workerId);
    Worker* findWorker(int workerId) const;
    bool hasWorker(int workerId) const;
    std::vector<Worker*> getWorkers() const;
    std::size_t getWorkersCount() const;

    double calculatePayroll() const;

    const std::string& getName() const;
    int getHeadcountLimit() const;
    double getMonthlyBudget() const;
    virtual std::string getDescription() const;

protected:
    std::string name_;
    std::string description_;
    int headcountLimit_ = kDefaultHeadcountLimit;
    double monthlyBudget_ = kDefaultMonthlyBudget;
    std::vector<std::unique_ptr<Worker>> workers_;
};
