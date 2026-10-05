# Система расчёта заработной платы (salary-counting-system)

Интерактивная C++20-система расчёта заработной платы сотрудников компании:
найм и увольнение, авансы, больничные и отпуска, переработки, налоговые
вычеты по прогрессивной шкале, выплаты по департаментам и итоговый отчёт
компании.

## Структура проекта

- `include/` — заголовочные файлы всех классов системы;
- `src/` — реализация классов;
- `tests/` — юнит-тесты (UnitTest++), 9 тестовых наборов;
- `main.cpp` — интерактивная консольная симуляция (меню из 9 пунктов);
- `.github/workflows/ci.yml` — CI: строгая сборка с `-Werror`, тесты,
  clang-format, поиск TODO и отчёт покрытия gcovr (порога ≥ 90 %).

## Классы

| Класс | Поля | Методы | Ассоциации |
|---|---|---|---|
| WorkerFactory |  | create, positionFor | AdvancePayment, EmploymentContract, Worker |
| CalculationContext | worker_, period_, calculationDate_ | getWorker, getPeriod, getCalculationDate, getBaseRate, getEffectiveRate, isOnProbation, isActive, getOvertimeHours, getTotalAbsentDays, getAveragePayRate, getAbsences, getAdvance, getContract | Absence, AdvancePayment, EmploymentContract, PayrollPeriod, Worker |
| Manager | managementBonusRate_ | getRole, generateReport, approveLeave | Worker |
| Specialist | specializationBonusRate_ | getRole, getSpecialization, getSpecializationBonusRate | Worker |
| Worker | kOvertimeLimitHours, id_, fullName_, contract_, advance_, department_, absences_, overtimeHours_ | getId, getFullName, getPosition, getHireDate, getBaseRate, getEffectiveRate, isOnProbation, isActive, calculateSalary, repayAdvance, resetAdvance, getRole, addAbsence, getAbsences, getTotalAbsentDays, getAveragePayRate, registerOvertime, getOvertimeHours, resetPeriod, setDepartment, getDepartment, worksIn, getContract, getAdvance | Absence, Accountant, Administrator, AdvancePayment, CustomerSupportAgent, Department, EmploymentContract, Lawyer, Marketer, PayrollAccountant, Recruiter, ResearchScientist, Salary, SecurityGuard, SoftwareDeveloper, SystemAdministrator, Technician |
| Absence | kFullPay, kNoPay, kHalfPay, days_, payRate_ | dayOff, getDays, getPayRate, getTypeName |  |
| SickLeave | kJuniorThresholdYears, kMiddleThresholdYears, kJuniorPayRate, kMiddlePayRate, seniorityYears_ | forDays, getSeniorityYears, getPayRate, getTypeName | Absence |
| Vacation |  | paid, unpaid, getTypeName | Absence |
| AdvancePayment | kRepaymentTermDays, amount_, repaidAmount_, issueDate_, repaymentDue_ | getAmount, getRepaidAmount, getRemaining, isFullyRepaid, getIssueDate, getRepaymentDue, applyDeduction |  |
| EmploymentContract | contractNumber_, position_, hireDate_, monthlyRate_, terminated_, renewalCount_, terminationDate_, probationPeriod_ | isActiveOn, isTerminated, isOnProbationOn, renew, terminate, getMonthlyRate, getEffectiveRateOn, getContractNumber, getPosition, getHireDate, getTerminationDate, getProbationPeriod, getRenewalCount | ProbationPeriod |
| PayrollPeriod | year_, month_, daysInMonth_ | getYear, getMonth, getFirstDay, getLastDay, contains, getDaysInMonth, toString |  |
| ProbationPeriod | DEFAULT_DURATION_DAYS, DISCOUNT_RATE, startDate_, durationDays_ | isActiveOn, isCompleted, daysRemaining, getDurationDays, getDiscountRate, calculateRate |  |
| Salary | gross_, taxDeduction_, net_, bonusesTotal_, deductionsTotal_ | getGross, getTaxDeduction, getNet, applyBonus, applyDeduction, applyRepayment, applyTax, getBonusesTotal, getDeductionsTotal, toString |  |
| Accountant | kBaseSpecialistRate, kSeniorCertifiedBonus, certificationLevel_ | getRole, getSpecialization, getCertificationLevel, isSeniorCertified | AdvancePayment, EmploymentContract, Specialist |
| PayrollAccountant | kPayrollBonus, kMaxEmployeesPerPayrollAccountant, employeesUnderService_ | getRole, getEmployeesUnderService, servesDepartment | Accountant, AdvancePayment, EmploymentContract |
| AccountingDepartment | kMaxHeadcount, kMonthlyBudget |  | Department |
| Administrator | kOfficeAdminBonus, kBonusTaskThreshold, officeBuilding_, tasksCompleted_ | getRole, getOfficeBuilding, getTasksCompleted, completeTask | AdvancePayment, EmploymentContract, Manager |
| AdministrationDepartment | kMaxHeadcount, kMonthlyBudget |  | Department |
| CustomerSupportAgent | kBaseSupportRate, kQualityBonus, kServiceStandardMinutes, supportChannel_, ticketsResolved_, totalResponseMinutes_ | getRole, getSpecialization, getSupportChannel, getTicketsResolved, getAverageResponseMinutes, resolveTicket, exceedsServiceStandard | AdvancePayment, EmploymentContract, Specialist |
| CustomerSupportDepartment | kMaxHeadcount, kMonthlyBudget |  | Department |
| Department | kDefaultHeadcountLimit, kDefaultMonthlyBudget, name_, description_, headcountLimit_, monthlyBudget_, workers_ | addWorker, removeWorker, findWorker, hasWorker, getWorkers, getWorkersCount, calculatePayroll, getName, getHeadcountLimit, getMonthlyBudget, getDescription | Worker |
| Recruiter | kBaseRecruiterRate, kHighClosingRateBonus, kTargetClosingRate, kMaxAdvanceRate, vacanciesClosed_, candidatesInProcess_ | getRole, getSpecialization, getVacanciesClosed, getCandidatesInProcess, registerCandidate, closeVacancy, getClosingRate, validateTerms, createContract, createAdvance, contractNumberFor | AdvancePayment, EmploymentContract, Specialist |
| HrDepartment | kMaxHeadcount, kMonthlyBudget, kName |  | Department |
| SoftwareDeveloper | kBaseDeveloperRate, kReviewBonus, primaryLanguage_, tasksCompleted_, codeReviewsGiven_ | getRole, getSpecialization, getPrimaryLanguage, getTasksCompleted, getCodeReviewsGiven, completeTask, reviewCode, isFullCycle | AdvancePayment, EmploymentContract, Specialist |
| SystemAdministrator | kBaseSysadminRate, kUptimeBonus, kCriticalDowntimePercent, serversMaintained_, uptimePercent_ | getRole, getSpecialization, getServersMaintained, getUptimePercent, recordUptime, addServer, hasCriticalDowntime | AdvancePayment, EmploymentContract, Specialist |
| ItDepartment | kMaxHeadcount, kMonthlyBudget |  | Department |
| Lawyer | kBaseLawyerRate, kCaseloadBonus, kCaseloadThreshold, barNumber_, casesHandled_ | getRole, getSpecialization, getBarNumber, getCasesHandled, handleCase, reviewsEmploymentContracts | AdvancePayment, EmploymentContract, Specialist |
| LegalDepartment | kMaxHeadcount, kMonthlyBudget |  | Department |
| Marketer | kBaseMarketerRate, kEfficiencyBonus, kTargetLeadsPerCampaign, campaignsLaunched_, leadsGenerated_ | getRole, getSpecialization, getCampaignsLaunched, getLeadsGenerated, launchCampaign, getLeadsPerCampaign | AdvancePayment, EmploymentContract, Specialist |
| MarketingDepartment | kMaxHeadcount, kMonthlyBudget |  | Department |
| Technician | kBaseTechnicianRate, kCertificationBonusPerCert, kOutputBonus, kOutputThreshold, kMultiCertifiedMinimum, equipmentCertifications_, unitsProduced_ | getRole, getSpecialization, getEquipmentCertifications, getUnitsProduced, produceUnits, isCertifiedOn, isMultiCertified | AdvancePayment, EmploymentContract, Specialist |
| ProductionDepartment | kMaxHeadcount, kMonthlyBudget |  | Department |
| ResearchScientist | kBaseResearchRate, kProjectBonus, kPublicationBonus, kProjectThreshold, kPublicationThreshold, researchArea_, projectsCompleted_, publicationsCount_ | getRole, getSpecialization, getResearchArea, getProjectsCompleted, getPublicationsCount, completeProject, publishPaper, hasPublicationRecord | AdvancePayment, EmploymentContract, Specialist |
| ResearchNDevelopementDepartment | kMaxHeadcount, kMonthlyBudget |  | Department |
| SecurityGuard | kBaseGuardRate, kNightShiftBonus, kIncidentBonus, kShiftThreshold, kIncidentBonusThreshold, kNightShift, shiftType_, shiftsCompleted_, incidentsPrevented_ | getRole, getSpecialization, getShiftType, getShiftsCompleted, getIncidentsPrevented, completeShift, preventIncident, isNightShift | AdvancePayment, EmploymentContract, Specialist |
| SecurityDepartment | kMaxHeadcount, kMonthlyBudget |  | Department |
| AbsencePayStrategy | kDaysPerMonth | apply | CalculationContext, DeductionStrategy, Salary |
| AdvanceDeductionStrategy |  | apply | CalculationContext, DeductionStrategy, Salary |
| DeductionStrategy |  | apply | CalculationContext, Salary |
| ProbationDiscountStrategy | kProbationDiscount | apply | CalculationContext, DeductionStrategy, Salary |
| TaxStrategy | kBracket1, kBracket2, kBracket3, kBracket4, kRate13, kRate15, kRate18, kRate20, kRate22 | apply | CalculationContext, DeductionStrategy, Salary |
| Company | name_, address_, taxId_, foundedYear_, industry_, ceoName_, website_, departments_, nextWorkerId_, balance_ | addDepartment, findDepartment, getDepartmentsCount, appointRecruiter, hireWorker, terminateWorker, registerAbsence, registerOvertime, endPeriod, addFunds, getBalance, calculateCompanyPayroll, getTotalWorkersCount, getAddress, getTaxId, getFoundedYear, getIndustry, getCeoName, getWebsite, generateReport | Absence, Department, Recruiter, Worker |

## Исключения (14)

Все исключения наследуются от `BaseException` (наследуется от
`std::runtime_error`) и образуют собственную иерархию:

- `BaseException` — базовый класс всех исключений системы, хранит текстовое сообщение об ошибке.
- `ContractExpiredException` (код 4001) — истёк срок трудового контракта.
- `DepartmentNotFoundException` (код 4002) — запрошенный отдел отсутствует в компании.
- `DuplicateEmployeeException` (код 4003) — попытка добавить сотрудника с уже занятым идентификатором.
- `EmployeeNotFoundException` (код 4004) — сотрудник с указанным идентификатором не найден в отделе.
- `FileIoException` (код 4005) — ошибка чтения или записи файла данных.
- `InsufficientFundsException` (код 4006) — на балансе компании недостаточно средств для выплаты.
- `InvalidContractException` (код 4007) — некорректная операция с контрактом (например, продление расторгнутого).
- `InvalidInputException` (код 4008) — переданы некорректные входные данные (пустое имя, неположительная сумма и т. п.).
- `OvertimeLimitExceededException` (код 4009) — запрошенный объём переработок превышает допустимый лимит.
- `PaymentFailedException` (код 4010) — выплата заработной платы не может быть выполнена.
- `SalaryCalculationException` (код 4011) — ошибка при расчёте заработной платы.
- `TaxCalculationException` (код 4012) — ошибка при расчёте налоговых удержаний.
- `UnauthorizedAccessException` (код 4013) — попытка доступа без необходимых прав.

### Итоговая статистика

| Классы | Поля | Поведения (методы) | Ассоциации | Исключения |
|---|---|---|---|---|
| 56 | 184 | 237 | 108 | 14 |

Методика подсчёта:

- **Поля** — все члены-данные классов (включая `static constexpr`-константы);
- **Поведения** — публичные методы (перегрузки считаются одним методом),
  конструкторы, деструкторы и операторы не учитываются;
- **Ассоциации** — для каждого класса: типы других классов, участвующие в его
  интерфейсе (поля, параметры методов, базовый класс, возвращаемые типы);
  итоговое число — сумма по всем классам;
- **Исключения** — собственные классы исключений проекта.

## Сборка и запуск

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build -j
ctest --test-dir build --output-on-failure   # 9 наборов тестов
./build/salary_sim                           # интерактивная симуляция
```
