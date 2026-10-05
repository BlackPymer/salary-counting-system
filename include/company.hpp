#pragma once

#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "departments/department.hpp"
#include "workers/worker.hpp"

class Recruiter;

class Company {
public:
    explicit Company(std::string name);
    Company(std::string name, std::string address, std::string taxId, int foundedYear,
            std::string industry, std::string ceoName, std::string website);

    void addDepartment(std::unique_ptr<Department> department);
    Department* findDepartment(const std::string& name) const;
    std::size_t getDepartmentsCount() const;

    Worker* appointRecruiter(const std::string& fullName, double monthlyRate);

    Worker* hireWorker(const std::string& departmentName, const std::string& fullName,
                       WorkerType type, double monthlyRate);

    std::unique_ptr<Worker> terminateWorker(const std::string& departmentName, int workerId);

    void registerAbsence(const std::string& departmentName, int workerId,
                         std::unique_ptr<Absence> absence);
    void registerOvertime(const std::string& departmentName, int workerId, double hours);

    void endPeriod();

    void addFunds(double amount);
    double getBalance() const;

    double calculateCompanyPayroll() const;
    int getTotalWorkersCount() const;

    const std::string& getAddress() const;
    const std::string& getTaxId() const;
    int getFoundedYear() const;
    const std::string& getIndustry() const;
    const std::string& getCeoName() const;
    const std::string& getWebsite() const;

    std::string generateReport() const;

private:
    Recruiter* findRecruiter() const;
    Worker* findWorkerIn(const std::string& departmentName, int workerId) const;
    void payWorker(Worker* worker);

    std::string name_;
    std::string address_;
    std::string taxId_;
    int foundedYear_ = 0;
    std::string industry_;
    std::string ceoName_;
    std::string website_;
    std::vector<std::unique_ptr<Department>> departments_;
    int nextWorkerId_ = 1;
    double balance_ = 0.0;
};
