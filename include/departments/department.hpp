#pragma once

#include <memory>
#include <string>
#include <vector>

#include "workers/worker.hpp"

class Department {
public:
    Department() = default;
    explicit Department(std::string name);
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
    virtual std::string getDescription() const;

protected:
    std::string name_;
    std::vector<std::unique_ptr<Worker>> workers_;
};
