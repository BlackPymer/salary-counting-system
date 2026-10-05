#include <UnitTest++/UnitTest++.h>

#include <memory>
#include <string>
#include <vector>

#include "departments/accounting/workers/accountant.hpp"
#include "departments/accounting/workers/payroll_accountant.hpp"
#include "departments/administration/workers/administrator.hpp"
#include "departments/customer_support/workers/customer_support_agent.hpp"
#include "departments/hr/workers/recruiter.hpp"
#include "departments/it/workers/software_developer.hpp"
#include "departments/it/workers/system_administrator.hpp"
#include "departments/legal/workers/lawyer.hpp"
#include "departments/marketing/workers/marketer.hpp"
#include "departments/production/workers/technician.hpp"
#include "departments/research_n_developement/workers/research_scientist.hpp"
#include "departments/security/workers/security_guard.hpp"
#include "exceptions/invalid_input_exception.hpp"

#include "test_helpers.hpp"

namespace {
constexpr double kTolerance = 0.001;
constexpr double kRate = 100000.0;
}  // namespace

TEST(AccountantTest_CertificationLevel) {
    Accountant a(1, "A", makeContract(kRate), makeAdvance(), "junior");
    CHECK_EQUAL("junior", a.getCertificationLevel());
    CHECK(!a.isSeniorCertified());

    Accountant s(2, "S", makeContract(kRate), makeAdvance(), "senior");
    CHECK(s.isSeniorCertified());
}

TEST(AccountantTest_CalculateSalaryWithSeniorBonus) {
    Accountant junior(1, "J", makeContract(kRate), makeAdvance(), "junior");
    Accountant senior(2, "S", makeContract(kRate), makeAdvance(), "senior");

    const double juniorBonus = junior.calculateSalary().getGross() - junior.getEffectiveRate();
    const double seniorBonus = senior.calculateSalary().getGross() - senior.getEffectiveRate();

    CHECK(seniorBonus > juniorBonus);
    CHECK_CLOSE(kRate * 0.10, juniorBonus, kTolerance);
    CHECK_CLOSE(kRate * 0.25, seniorBonus, kTolerance);
}

TEST(PayrollAccountantTest_ServesDepartment) {
    PayrollAccountant p(1, "P", makeContract(kRate), makeAdvance(), "chief", 30);
    CHECK_EQUAL(30, p.getEmployeesUnderService());
    CHECK(p.servesDepartment());

    PayrollAccountant many(2, "M", makeContract(kRate), makeAdvance(), "junior", 60);
    CHECK(!many.servesDepartment());
}

TEST(PayrollAccountantTest_BonusPaidUnconditionally) {
    PayrollAccountant serves(1, "P", makeContract(kRate), makeAdvance(), "junior", 30);
    PayrollAccountant many(2, "M", makeContract(kRate), makeAdvance(), "junior", 60);

    const double bonusServes = serves.calculateSalary().getGross() - serves.getEffectiveRate();
    const double bonusMany = many.calculateSalary().getGross() - many.getEffectiveRate();

    CHECK(bonusServes > 0.0);
    CHECK_CLOSE(bonusServes, bonusMany, kTolerance);
}

TEST(AdministratorTest_OfficeBuilding) {
    Administrator a(1, "A", makeContract(kRate), makeAdvance(), "B-1");
    CHECK_EQUAL("B-1", a.getOfficeBuilding());
    CHECK_EQUAL(0, a.getTasksCompleted());
}

TEST(AdministratorTest_CompleteTask) {
    Administrator a(1, "A", makeContract(kRate), makeAdvance(), "B-1");
    a.completeTask();
    a.completeTask();
    CHECK_EQUAL(2, a.getTasksCompleted());
}

TEST(CustomerSupportAgentTest_SupportChannel) {
    CustomerSupportAgent c(1, "C", makeContract(kRate), makeAdvance(), "chat");
    CHECK_EQUAL("chat", c.getSupportChannel());
    CHECK_EQUAL(0, c.getTicketsResolved());
    CHECK_EQUAL(0, c.getAverageResponseMinutes());
}

TEST(CustomerSupportAgentTest_ResolveTicket) {
    CustomerSupportAgent c(1, "C", makeContract(kRate), makeAdvance(), "chat");
    c.resolveTicket(10);
    c.resolveTicket(20);
    CHECK_EQUAL(2, c.getTicketsResolved());
    CHECK_EQUAL(15, c.getAverageResponseMinutes());
    CHECK(c.exceedsServiceStandard());
}

TEST(CustomerSupportAgentTest_ExceedsServiceStandard) {
    CustomerSupportAgent c(1, "C", makeContract(kRate), makeAdvance(), "chat");
    c.resolveTicket(120);
    CHECK(!c.exceedsServiceStandard());
}

TEST(SoftwareDeveloperTest_LanguageAndTasks) {
    SoftwareDeveloper d(1, "D", makeContract(kRate), makeAdvance(), "C++");
    CHECK_EQUAL("C++", d.getPrimaryLanguage());
    CHECK_EQUAL(0, d.getTasksCompleted());
    CHECK_EQUAL(0, d.getCodeReviewsGiven());
    CHECK(!d.isFullCycle());

    d.completeTask();
    CHECK_EQUAL(1, d.getTasksCompleted());
    CHECK(!d.isFullCycle());

    d.reviewCode();
    CHECK_EQUAL(1, d.getCodeReviewsGiven());
    CHECK(d.isFullCycle());
}

TEST(SystemAdministratorTest_ServersAndUptime) {
    SystemAdministrator s(1, "S", makeContract(kRate), makeAdvance(), 4);
    CHECK_EQUAL(4, s.getServersMaintained());
    CHECK_CLOSE(100.0, s.getUptimePercent(), kTolerance);
    CHECK(!s.hasCriticalDowntime());

    s.addServer();
    CHECK_EQUAL(5, s.getServersMaintained());
}

TEST(SystemAdministratorTest_RecordUptime) {
    SystemAdministrator s(1, "S", makeContract(kRate), makeAdvance(), 4);
    s.recordUptime(95.0);
    CHECK(s.hasCriticalDowntime());
    CHECK_CLOSE(95.0, s.getUptimePercent(), kTolerance);

    CHECK_THROW(s.recordUptime(101.0), InvalidInputException);
    CHECK_THROW(s.recordUptime(-1.0), InvalidInputException);
}

TEST(LawyerTest_BarNumberAndCases) {
    Lawyer l(1, "L", makeContract(kRate), makeAdvance(), "77-123");
    CHECK_EQUAL("77-123", l.getBarNumber());
    CHECK_EQUAL(0, l.getCasesHandled());

    l.handleCase();
    l.handleCase();
    CHECK_EQUAL(2, l.getCasesHandled());
    CHECK(l.reviewsEmploymentContracts());
}

TEST(LawyerTest_CaseloadBonus) {
    Lawyer l(1, "L", makeContract(kRate), makeAdvance(), "77-123");
    const double bonusBefore = l.calculateSalary().getGross() - l.getEffectiveRate();
    for (int i = 0; i < 5; ++i) {
        l.handleCase();
    }
    const double bonusAfter = l.calculateSalary().getGross() - l.getEffectiveRate();
    CHECK(bonusAfter > bonusBefore);
}

TEST(LawyerTest_ReviewsContracts) {
    Lawyer l(1, "L", makeContract(kRate), makeAdvance(), "77-123");
    l.handleCase();
    CHECK(l.reviewsEmploymentContracts());
}

TEST(MarketerTest_CampaignsAndLeads) {
    Marketer m(1, "M", makeContract(kRate), makeAdvance());
    CHECK_EQUAL(0, m.getCampaignsLaunched());
    CHECK_EQUAL(0, m.getLeadsGenerated());

    m.launchCampaign(100);
    m.launchCampaign(200);
    CHECK_EQUAL(2, m.getCampaignsLaunched());
    CHECK_EQUAL(300, m.getLeadsGenerated());
    CHECK_CLOSE(150.0, m.getLeadsPerCampaign(), kTolerance);
}

TEST(MarketerTest_NoCampaignsNoAverage) {
    Marketer m(1, "M", makeContract(kRate), makeAdvance());
    CHECK_CLOSE(0.0, m.getLeadsPerCampaign(), kTolerance);
}

TEST(TechnicianTest_Certifications) {
    Technician t(1, "T", makeContract(kRate), makeAdvance(), {"CNC", "Lathe"});
    CHECK_EQUAL(2u, t.getEquipmentCertifications().size());
    CHECK(t.isCertifiedOn("CNC"));
    CHECK(t.isCertifiedOn("Lathe"));
    CHECK(!t.isCertifiedOn("Welder"));
    CHECK(t.isMultiCertified());
}

TEST(TechnicianTest_ProduceUnits) {
    Technician t(1, "T", makeContract(kRate), makeAdvance(), {"CNC"});
    CHECK_EQUAL(0, t.getUnitsProduced());
    t.produceUnits(10);
    t.produceUnits(5);
    CHECK_EQUAL(15, t.getUnitsProduced());
}

TEST(TechnicianTest_SingleCertification) {
    Technician t(1, "T", makeContract(kRate), makeAdvance(), {"CNC"});
    CHECK(!t.isMultiCertified());
}

TEST(ResearchScientistTest_ResearchArea) {
    ResearchScientist r(1, "R", makeContract(kRate), makeAdvance(), "materials");
    CHECK_EQUAL("materials", r.getResearchArea());
    CHECK_EQUAL(0, r.getProjectsCompleted());
    CHECK_EQUAL(0, r.getPublicationsCount());
    CHECK(!r.hasPublicationRecord());
}

TEST(ResearchScientistTest_CompleteAndPublish) {
    ResearchScientist r(1, "R", makeContract(kRate), makeAdvance(), "materials");
    r.completeProject();
    r.completeProject();
    CHECK_EQUAL(2, r.getProjectsCompleted());

    r.publishPaper();
    CHECK_EQUAL(1, r.getPublicationsCount());
    CHECK(r.hasPublicationRecord());
}

TEST(SecurityGuardTest_ShiftType) {
    SecurityGuard day(1, "D", makeContract(kRate), makeAdvance(), "day");
    SecurityGuard night(2, "N", makeContract(kRate), makeAdvance(), "night");

    CHECK_EQUAL("day", day.getShiftType());
    CHECK(!day.isNightShift());
    CHECK(night.isNightShift());
}

TEST(SecurityGuardTest_ShiftsAndIncidents) {
    SecurityGuard s(1, "S", makeContract(kRate), makeAdvance(), "night");
    CHECK_EQUAL(0, s.getShiftsCompleted());
    CHECK_EQUAL(0, s.getIncidentsPrevented());

    s.completeShift();
    s.completeShift();
    s.preventIncident();
    CHECK_EQUAL(2, s.getShiftsCompleted());
    CHECK_EQUAL(1, s.getIncidentsPrevented());
}

TEST(SecurityGuardTest_BonusHigherOnNightShift) {
    SecurityGuard day(1, "D", makeContract(kRate), makeAdvance(), "day");
    SecurityGuard night(2, "N", makeContract(kRate), makeAdvance(), "night");

    const double dayBonus = day.calculateSalary().getGross() - day.getEffectiveRate();
    const double nightBonus = night.calculateSalary().getGross() - night.getEffectiveRate();

    CHECK(nightBonus > dayBonus);
}
