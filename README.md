# **Personal Finance Manager — CS160 Group 02 Final Project**

> **Course:** CS160 - Introduction to Programming (APCS)  
> **Semester**: HK1 2026-2027
> **Language**: C++17
>**GitHub Repository**: https://github.com/thvy2507/CS160-Group02-FinalProject
> **Status:** In Development
> **Team size**: 4 members | **Time**: 5 weeks

---

## 1. Thành viên và Vai trò
| Thành viên | Vai trò | Module chính |
|---|---|---|
| TV1 | Team Lead / Core & Integration | DynamicArray, Date, FinanceManager, File Persistence, tích hợp CLI |
| TV2 | Master Data & Wallets | Wallet, Income Source, Expense Category, CRUD, cập nhật số dư |
| TV3 | Transactions & Recurring | Income, Expense, TransactionManager, giao dịch định kỳ |
| TV4 | Statistics & Dashboard | Báo cáo theo thời gian, ví, năm, phân loại và Dashboard |

→ Chi tiết: xem [`ROLES.md`](ROLES.md)  
→ Kế hoạch tuần: xem [WEEKLY_PLAN.md](WEEKLY_PLAN.md)

---

## 2. Tech stack

| Thành phần | Chi tiết |
|---|---|
| Ngôn ngữ | C++17 |
| Giao diện | Console / CLI |
| Data Structures | Custom Dynamic Array    |
| File Handling | `fstream` (Binary) |
| Build | CMake 3.15+ |
| Compiler | Clang (macOS) / g++ hoặc MSVC (Windows) |
| Version control | Git + GitHub |
---

## 3. Cài đặt & build

### 1. Clone repository

```bash
git clone https://github.com/thvy2507/CS160-Group02-FinalProject.git
cd CS160-Group02-FinalProject
```

### 2. Build

```bash
cmake -S . -B build
cmake --build build
```

---

## 4. Cấu trúc thư mục tóm tắt

```
CS160-Group02-FinalProject/
├── include/         # Các file header (.h) phân theo module
├── src/             # File source (.cpp) tương ứng với include/
├── data/            # Binary data files and Name Pools
├── tests/   	     # Test từng module
├── docs/
└── CMakeLists.txt   # File cấu hình build CMake

```

Chi tiết cấu trúc file và quy định đặt tên, xem tại [`FILE_STRUCTURE.md`](FILE_STRUCTURE.md).

---

## 5. Tài liệu liên quan

- [ROLES.md](ROLES.md) — Phân vai chi tiết
- [WEEKLY_PLAN.md](WEEKLY_PLAN.md) — Kế hoạch 5 tuần theo từng thành viên
- [FILE_STRUCTURE.md](FILE_STRUCTURE.md) — Cấu trúc thư mục & file
- [CODING_RULES.md](CODING_RULES.md) — Quy tắc code, Git, naming
