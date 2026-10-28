# **Kế hoạch 5 tuần — Personal Finance Manager**

> **Mục tiêu:** Hoàn thành ứng dụng Personal Finance Manager bằng C++ theo yêu cầu của đề bài.
>
> **Demo checkpoint cuối mỗi tuần:** Cả nhóm build và kiểm thử các chức năng đã hoàn thành. Không chấp nhận chức năng được đánh dấu hoàn thành nhưng chưa chạy thử.

---

## Tuần 1 — Setup & Architecture

**Demo checkpoint:** Mọi người clone repo → build thành công bằng CMake trên macOS/Windows → chạy chương trình → hiển thị menu CLI cơ bản.

| | TV1 (Core) | TV2 (Master Data) | TV3 (Transactions) | TV4 (Statistics) |
|---|---|---|---|---|
| **Việc chính** | Setup project và kiến trúc tổng thể | Thiết kế Master Data | Thiết kế Transaction Models | Thiết kế Statistics & Dashboard |
| **Chi tiết** | Thiết lập CMake, cấu trúc thư mục, `DynamicArray`, `Date`, khung `FinanceManager`; thống nhất interface và thiết kế sơ bộ Binary format (fixed-size records, Name Pool, offset/length). | Thiết kế `Wallet`, `IncomeSource`, `ExpenseCategory` và các Manager; thống nhất ID/Name | Thiết kế `Transaction`, `Income`, `Expense`, `RecurringTransaction`; thống nhất cách liên kết với Wallet | Thiết kế các loại báo cáo, dữ liệu cần lấy từ TV2/TV3 và bố cục Dashboard |
| **Deliverable** | Project build được, CLI skeleton, Core Utilities cơ bản | Data models và interface của các Manager | Transaction models và interface | Statistics/Dashboard skeleton, danh sách báo cáo và test cases mẫu |
| **Thời gian dự kiến** | ~8–10h | ~5–7h | ~5–7h | ~5–7h |



---

## Tuần 2 — Core Management

**Demo checkpoint:** Chạy chương trình → tạo ví, nguồn thu, danh mục → thêm giao dịch thu nhập/chi tiêu → kiểm tra số dư ví được cập nhật chính xác.

| | TV1 (Core) | TV2 (Master Data) | TV3 (Transactions) | TV4 (Statistics) |
|---|---|---|---|---|
| **Việc chính** | Hoàn thiện Core và tích hợp CLI | Implement Master Data CRUD | Implement Income & Expense | Implement thống kê cơ bản |
| **Chi tiết** | Hoàn thiện `DynamicArray`, `Date`, menu và input validation; kết nối các module | Thêm, sửa, xóa, hiển thị Wallet/IncomeSource/ExpenseCategory; quản lý ID và số dư ví | Thêm, hiển thị giao dịch; kiểm tra dữ liệu; phối hợp TV2 cập nhật số dư | Tính tổng thu nhập, chi tiêu và Net Balance theo khoảng thời gian; thử nghiệm với dữ liệu mẫu |
| **Deliverable** | CLI gọi được các chức năng đã implement | CRUD hoạt động trong RAM, số dư ví chính xác. | Income/Expense hoạt động trong RAM và liên kết đúng ID. | Time-based Statistics hoạt động |
| **Thời gian dự kiến** | ~8–11h | ~12-15h | ~11–14h | ~7–10h |

---

## Tuần 3 — Persistence & Recurring Transactions

**Demo checkpoint:** Các module thực hiện được đọc/ghi Binary cơ bản với dữ liệu mẫu. Kiểm tra fixed-size records và Name Pool bước đầu. Recurring Transactions hoạt động đúng chu kỳ trong RAM và không tạo trùng.

| | TV1 (Core) | TV2 (Master Data) | TV3 (Transactions) | TV4 (Statistics) |
|---|---|---|---|---|
| **Việc chính** | Data Persistence | Hoàn thiện lưu/đọc Master Data | Recurring Transactions | Wallet-based Statistics |
| **Chi tiết** | Thiết kế và triển khai Binary File I/O cơ bản; thống nhất record format, offset/length và cơ chế Name Pool; điều phối save/load bước đầu. | Bắt đầu implement đọc/ghi Binary cho Wallet, IncomeSource, ExpenseCategory và các Name Pool tương ứng. | Implement recurring hằng tháng, Start/End Date, chống trùng; bắt đầu đọc/ghi Binary cho Transactions và Recurring. | Thống kê thu nhập/chi tiêu theo từng ví trong khoảng thời gian được chọn |
| **Deliverable** | Binary Persistence cơ bản hoạt động với dữ liệu mẫu. | Master Data đọc/ghi Binary cơ bản, thử nghiệm Name Pool. | Recurring hoạt động trong RAM; đọc/ghi Binary bước đầu.      | Wallet-based Statistics hoạt động |
| **Thời gian dự kiến** | ~10–13h | ~8-11h | ~12–16h                                                      | ~8-11h |

---

## Tuần 4 — Full Statistics & Dashboard

**Demo checkpoint:** Khởi động ứng dụng → Dashboard hiển thị tổng số dư và số dư từng ví → truy cập được tất cả báo cáo. Thêm, sửa, xóa dữ liệu → thoát chương trình → mở lại → dữ liệu được khôi phục chính xác từ các file Binary.

| | TV1 (Core) | TV2 (Master Data) | TV3 (Transactions) | TV4 (Statistics) |
|---|---|---|---|---|
| **Việc chính** | Tích hợp toàn bộ chức năng | Hoàn thiện Master Data & Wallets | Hoàn thiện Transactions | Full Statistics & Dashboard |
| **Chi tiết** | Hoàn thiện Binary Persistence, random access và tích hợp save/load toàn ứng dụng; hoàn thiện navigation flow. | Hoàn thiện Rename/Delete với fixed-size records và Name Pool; xử lý dữ liệu đang được tham chiếu và kiểm tra số dư. | Hoàn thiện Binary Persistence cho Transactions và Recurring; kiểm thử chống trùng, các tháng bị bỏ lỡ và tính chính xác giao dịch. | Annual Overview cho một/nhiều năm; Income/Expense Breakdown; hoàn thiện Dashboard và định dạng số liệu |
| **Deliverable** | Các module tích hợp và save/load Binary hoạt động chính xác. | Master Data CRUD và Name Pool được lưu/khôi phục chính xác. | Transactions & Recurring được lưu/khôi phục chính xác. | Dashboard và toàn bộ báo cáo theo yêu cầu |
| **Thời gian dự kiến** | ~10–13h | ~7–10h | ~9–12h | ~13-17h |

---

## Tuần 5 — Testing, Documentation & Submission

**Demo checkpoint:** Clone repo trên máy khác → build thành công → chạy thử toàn bộ chức năng → đóng/mở ứng dụng không mất dữ liệu → báo cáo và video demo hoàn chỉnh.

| | TV1 (Core) | TV2 (Master Data) | TV3 (Transactions) | TV4 (Statistics) |
|---|---|---|---|---|
| **Việc chính** | Final Integration & Code Review | Master Data Testing | Transaction Testing | Statistics Testing |
| **Chi tiết** | Kiểm tra requirements, review code, clean build trên macOS/Windows; kiểm thử toàn bộ Binary Persistence và xử lý lỗi file; tổng hợp tài liệu. | Test CRUD, ID, ví, số dư, Rename/Delete, Name Pool, save/load qua nhiều phiên chạy; sửa lỗi module. | Test Income/Expense, recurring, ngày tháng, chống trùng và lưu/đọc Binary qua nhiều phiên chạy; sửa lỗi module. | Đối chiếu kết quả báo cáo bằng dữ liệu mẫu, kiểm tra Dashboard và các trường hợp không có dữ liệu |
| **Deliverable** | Source ổn định, báo cáo kiến trúc và bản build hoàn chỉnh | Module hoàn thiện, kết quả kiểm thử | Module hoàn thiện, kết quả kiểm thử | Báo cáo chính xác, kết quả kiểm thử |
| **Thời gian dự kiến** | ~12–16h | ~10-13h | ~10–13h | ~10–13h |

**Công việc chung cuối tuần 5:**
- Hoàn thiện project report và tài liệu phân công công việc.
- Tổng hợp lịch sử commit, Pull Request và đóng góp của từng thành viên.
- Quay video demo các chức năng.
- Kiểm tra source code, README và các file cần nộp.
- Đóng gói project theo đúng yêu cầu giảng viên.

---

## Tóm tắt Milestone

```text
Tuần 1 │ ▪ Setup, CMake, Architecture, CLI Skeleton
Tuần 2 │ ▪ Master Data CRUD + Income/Expense
Tuần 3 │ ▪ Data Persistence + Recurring + Wallet Statistics
Tuần 4 │ ▪ Full Statistics + Dashboard + Integration
Tuần 5 │ ▪ Testing + Documentation + Final Submission
```
