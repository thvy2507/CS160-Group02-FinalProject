# Weekly Report — Week 05

### General Information

- **Group ID**: Group02
- **Project Name**: Personal Finance Manager
- **Course:** CS160-26A01
- **Date Range:** 28/10/2026 – 06/11/2026
- **GitHub Repository:** https://github.com/thvy2507/CS160-Group02-FinalProject

---

## Team Members

| Member | Name               | Responsibility                   |
| ------ | ------------------ | -------------------------------- |
| TV1    | Trần Hồng Vy       | Team Leader / Core & Integration |
| TV2    | Lê Thiện Khôi      | Master Data & Wallets            |
| TV3    | Dương Thị Nhã Uyên | Transactions & Recurring         |
| TV4    | Trần Nhật Tân      | Statistics & Dashboard           |

---

## Tasks Completed This Week

> During this initial reporting period, the team primarily focused on project planning, documentation, and establishing the development workflow. Due to **midterm examinations**, implementation tasks were temporarily postponed.

### 1. Project Planning & Documentation

- Prepared the project documentation, including:
  - `README.md`
  - `ROLES.md`
  - `WEEKLY_PLAN.md`
  - `FILE_STRUCTURE.md`
  - `CODING_RULES.md`
  - `docs/plans/plan_for_week_1.md`
- Established a five-week development roadmap with weekly objectives, deliverables, and demo checkpoints.
- Defined the responsibilities of all four team members.
- Prepared the initial Week 1 task assignments.

### 2. Architecture & Technology Decisions

- Selected C++17 and a console-based interface (CLI).
- Chose CMake as the cross-platform build system.
- Planned the use of a custom Dynamic Array for managing collections.
- Selected Binary File Storage using `<fstream>`.
- Discussed the preliminary binary storage design, including fixed-size records and separate Name Pools.
- Defined the proposed directory structure and module boundaries.

### 3. Git & Collaboration Setup

- Established the GitHub repository for collaboration.
- Agreed on a workflow using individual feature branches and Pull Requests into `main`.
- Assigned TV1 as the person responsible for reviewing and merging Pull Requests.
- Established commit message conventions and coding rules.
- Updated the weekly workflow to accommodate the Friday reporting deadline.

**Implementation Status:** Core functionality, including CMake setup, CLI skeleton, data models, and manager interfaces, remains planned work unless separately completed and verified.

---

## AI Usage Declaration

ChatGPT was used to support project planning and documentation during this reporting period.

The assistance included:

- Reviewing and organizing the project's functional requirements.
- Discussing the responsibilities of each team member.
- Drafting and refining the five-week development plan.
- Preparing documentation structure and Git workflow guidelines.
- Reviewing the Week 1 task plan.

All AI-assisted documentation should be reviewed by the team before submission.

Detailed usage records and relevant chat logs are provided separately in `AI Usage Declaration.md`.

---

## Tasks Planned for Next Week

The team plans to proceed with the initial implementation tasks described in `docs/plans/plan_for_week_1.md`.

**TV1 — Trần Hồng Vy (Core & Integration)**

- Set up the project directory structure and CMake configuration.
- Implement the initial `FinanceManager` CLI skeleton.
- Begin implementing `DynamicArray` and `Date` utilities.
- Coordinate shared interfaces and preliminary binary storage design.

**TV2 — Khôi (Master Data & Wallets)**

- Design `Wallet`, `IncomeSource`, and `ExpenseCategory` models.
- Prepare the corresponding Manager interfaces.
- Define ID management and wallet balance operations.

**TV3 — Uyên (Transactions & Recurring)**

- Design `Transaction`, `Income`, `Expense`, and `RecurringTransaction` models.
- Prepare the Transaction and Recurring Manager interfaces.
- Define the relationships between transactions and master data.

**TV4 — Tân (Statistics & Dashboard)**

- Define the required financial reports.
- Prepare the Statistics and Dashboard skeletons.
- Create sample datasets and expected results for future testing.

---

## Issues

### Issue 1: Limited Development Time Due to Midterm Examinations

Team members had to allocate time to university midterm examinations, reducing the time available for project implementation.

**How Addressed:**

- Prioritized project planning and documentation.
- Divided the project into independent modules to support parallel development.
- Prepared implementation tasks for the following development period.
- Planned to record unfinished work accurately rather than marking it as completed.

### Issue 2: Dependencies Between Modules

Several components depend on shared data models and interfaces. For example, Transaction Management requires Wallet, Income Source, and Expense Category information, while Statistics depends on transaction and wallet data.

**How Addressed:**

- Assigned clear ownership of each module.
- Planned to establish common interfaces before implementing dependent features.
- Required team members to communicate interface changes before integration.

### Issue 3: Weekly Reporting Deadline

The Weekly Report must be submitted on Friday, which affects the original end-of-week integration schedule.

**How Addressed:**

- Scheduled Pull Request reviews and integration for Thursday.
- Scheduled progress verification and report preparation for Friday.
- Allowed incomplete tasks to remain in progress and be documented in the report.
