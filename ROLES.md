# **Phân vai nhóm — 4 thành viên**

> Mỗi thành viên phụ trách một module chính: tự thiết kế, implement và kiểm thử module của mình.
> Khi cần phối hợp qua module khác → trao đổi trước, không tự ý sửa code của người khác.

---

## Tổng quan phân công

| Thành viên | Vai trò | Module chính |
|---|---|---|
| TV1 | Leader / Core & Integration | DynamicArray, Date, FinanceManager, File Persistence, tích hợp CLI |
| TV2 | Master Data & Wallets | Wallet, Income Source, Expense Category, CRUD, cập nhật số dư |
| TV3 | Transactions & Recurring | Income, Expense, TransactionManager, giao dịch định kỳ |
| TV4 | Statistics & Dashboard | Báo cáo theo thời gian, ví, năm, phân loại và Dashboard |

---

## TV1 — Leader / Core & Integration

**Trách nhiệm chính:**

- Thiết kế kiến trúc tổng thể và thống nhất interface giữa các module.
- Xây dựng `DynamicArray` và các tiện ích dùng chung như `Date`.
- Quản lý luồng hoạt động chính của chương trình (`FinanceManager`).
- Phụ trách hệ thống lưu và đọc dữ liệu (Binary).
- Thiết lập CMake để project build được trên macOS và Windows.
- Tích hợp các module thành ứng dụng hoàn chỉnh.

**Module sở hữu:**

- Core Utilities (`DynamicArray`, `Date`)
- Application Controller (`FinanceManager`)
- Data Persistence
- Build Configuration

---

## TV2 — Master Data & Wallets

**Trách nhiệm chính:**

- Quản lý danh sách ví tiền (`Wallet`).
- Quản lý nguồn thu nhập (`IncomeSource`).
- Quản lý danh mục chi tiêu (`ExpenseCategory`).
- Implement các chức năng thêm, sửa, xóa và hiển thị dữ liệu (CRUD).
- Quản lý số dư ví và hỗ trợ cập nhật số dư khi phát sinh giao dịch.
- Đảm bảo ID được lưu trữ và Name được hiển thị chính xác.

**Module sở hữu:**

- `Wallet` / `WalletManager`
- `IncomeSource` / `IncomeSourceManager`
- `ExpenseCategory` / `ExpenseCategoryManager`

---

## TV3 — Transactions & Recurring

**Trách nhiệm chính:**

- Quản lý các giao dịch thu nhập (`Income`) và chi tiêu (`Expense`).
- Xử lý thông tin giao dịch: ngày, số tiền, ví, nguồn thu/danh mục và mô tả.
- Kiểm tra dữ liệu đầu vào và liên kết giao dịch với ví.
- Implement giao dịch định kỳ hằng tháng (`RecurringTransaction`).
- Tự động phát sinh giao dịch đến hạn và tránh tạo giao dịch trùng lặp.
- Phối hợp với TV2 để cập nhật số dư ví chính xác.

**Module sở hữu:**

- `Transaction`
- `Income` / `Expense`
- `TransactionManager`
- `RecurringTransaction`

---

## TV4 — Statistics & Dashboard

**Trách nhiệm chính:**

- Thống kê tổng thu nhập, chi tiêu và số dư ròng theo khoảng thời gian.
- Thống kê thu nhập và chi tiêu theo từng ví.
- Xây dựng báo cáo tài chính cho một hoặc nhiều năm.
- Thống kê thu nhập theo nguồn thu và chi tiêu theo danh mục.
- Xây dựng Dashboard hiển thị tổng số dư và số dư từng ví.
- Định dạng số liệu và trình bày báo cáo rõ ràng trên Console.

**Module sở hữu:**

- `StatisticsManager`
- `Dashboard`
- Number Formatting Utilities

---

## Quy tắc phối hợp

1. **Không commit vào module của người khác** khi chưa trao đổi trước.
2. **Interface thay đổi → thông báo cả nhóm** trước khi merge vào `main`.
3. **Mỗi thành viên làm việc trên branch riêng**, tạo Pull Request vào `main` sau khi hoàn thành và kiểm thử.
4. **Chỉ TV1 được quyền merge code vào `main`**. Các thành viên khác có thể review và góp ý nhưng không được tự merge.
5. **Cuối tuần Chủ nhật:** cả nhóm pull `main`, build và kiểm thử chương trình, sau đó cập nhật tiến độ.
6. **TV1 có quyền quyết định** khi xảy ra conflict về design. Ý kiến khác → raise trong meeting, không tự sửa.
