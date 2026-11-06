# **Phân Công Công Việc — Week 1: Setup & Architecture**

> **Mục tiêu:** Thiết lập project, hoàn thiện cấu trúc thư mục và thống nhất kiến trúc, data models, interface giữa các module.
>
> **Demo checkpoint:** Mọi người clone repo → build thành công bằng CMake trên macOS/Windows → chạy executable → hiển thị menu CLI cơ bản mà không gặp lỗi.

## Quy trình Git của cả team

Mỗi người làm việc trên **branch riêng**, không commit thẳng vào `main`:

```
main          (ổn định, merge cuối tuần)
├── feat/core-setup               ← Vy
├── feat/master-data-models       ← Khôi
├── feat/transaction-models       ← Uyên
├── feat/statistics-design        ← T

```

**Các bước cho mỗi người:**

```bash
# 1. Cập nhật main và tạo branch riêng
git switch main
git pull origin main
git switch -c feat/ten-branch-cua-ban

# 2. Code, test và commit thường xuyên
git add .
git commit -m "[TVx] feat: short description"
git push -u origin feat/ten-branch-cua-ban

# 3. Hoàn thành → tạo Pull Request vào main trên GitHub
# 4. Vy review, kiểm tra build và merge PR vào main
```

> **Thứ Năm**: Vy review và merge các Pull Request hợp lệ vào `main`.
>**Thứ Sáu:** Cả nhóm pull `main`, build, kiểm thử, chạy demo checkpoint và hoàn thành Weekly Report trước hạn nộp.

---

## Những gì đã chuẩn bị

| Hạng mục                          | Trạng thái    |
| --------------------------------- | ------------- |
| `README.md`                       | Đã soạn       |
| `ROLES.md`                        | Đã soạn       |
| `WEEKLY_PLAN.md`                  | Đã soạn       |
| `FILE_STRUCTURE.md`               | Đã soạn       |
| `CODING_RULES.md`                 | Đã soạn       |
| Phân công 4 module chính          | Đã thống nhất |
| C++17, CMake, Binary Storage      | Đã thống nhất |
| Git workflow và commit convention | Đã thống nhất |

---

## Việc cần làm — Từng thành viên

### 🔴 TV1 (Vy) — Leader / Core & Integration

**Branch:** `feat/core-setup`

**Thời gian dự kiến:** ~8–10 giờ

- Thiết lập cấu trúc thư mục `include/`, `src/`, `tests/`, `data/` theo `FILE_STRUCTURE.md`.
- Tạo `CMakeLists.txt`:
  - Sử dụng C++17.
  - Cấu hình đường dẫn `include/`.
  - Build được trên macOS và Windows.
- Tạo `src/main.cpp`, `include/core/FinanceManager.h` và `src/core/FinanceManager.cpp`:
  - `main.cpp` chỉ khởi tạo và chạy `FinanceManager`.
  - `FinanceManager::run()` hiển thị menu CLI cơ bản.
  - Có thể thoát chương trình bình thường.
- Viết phiên bản cơ bản của `include/core/DynamicArray.h`:
  - Constructor và Destructor.
  - Quản lý cấp phát động bằng `new[]` / `delete[]`.
  - Thao tác thêm phần tử, truy cập phần tử và lấy kích thước.
  - Xử lý copy an toàn, tránh shallow copy.
- Tạo `include/core/Date.h` và `src/core/Date.cpp`:
  - Cấu trúc ngày/tháng/năm.
  - Khai báo các hàm xử lý và kiểm tra ngày hợp lệ.
- Chủ trì thống nhất interface và dependency giữa các module.
- Thống nhất thiết kế sơ bộ Binary Storage:
  - Fixed-size records.
  - Name Pool, offset và length.
  - Phân biệt record index và object ID.
  - Chưa cần implement File I/O trong tuần này.
- Review và merge các Pull Request sau khi kiểm tra.

**Deliverable:**

- Project skeleton build thành công.
- CLI cơ bản hoạt động.
- `DynamicArray` và `Date` phiên bản đầu.
- Shared interfaces được thống nhất.

### 🟢 TV2 (Khôi) — Master Data & Wallets

**Branch:** `feat/master-data-models`

**Thời gian dự kiến:** ~5–7 giờ

- Thiết kế và tạo các data models:
  - `include/models/Wallet.h`
  - `include/models/IncomeSource.h`
  - `include/models/ExpenseCategory.h`
- Xác định các thuộc tính cần thiết cho từng model:
  - ID và Name.
  - Wallet balance.
  - Các thông tin cần thiết để phục vụ CRUD và giao dịch.
- Tạo interface cho các manager:
  - `include/managers/WalletManager.h`
  - `include/managers/IncomeSourceManager.h`
  - `include/managers/ExpenseCategoryManager.h`
- Khai báo các thao tác chính:
  - Add, View, Edit, Delete.
  - Tìm kiếm theo ID.
  - Cập nhật số dư ví.
- Thống nhất với TV3 (Uyên) cách truy cập Wallet, IncomeSource và ExpenseCategory bằng ID.
- Thảo luận với TV1 (Vy) về fixed-size records và các Name Pool tương ứng.
- Tạo `.cpp` skeleton nếu cần cho các class đã khai báo.

**Deliverable:**

- Master Data models.
- Interface của các Manager.
- Thiết kế ID/Name và quy tắc quản lý Wallet Balance.

> Chưa cần implement toàn bộ CRUD hoặc đọc/ghi Binary trong Week 1.

### 🔵 TV3 (Uyên) — Transactions & Recurring

**Branch:** `feat/transaction-models`

**Thời gian dự kiến:** ~5–7 giờ

- Thiết kế và tạo các data models:
  - `include/models/Transaction.h`
  - `include/models/Income.h`
  - `include/models/Expense.h`
  - `include/models/RecurringTransaction.h`
- Thống nhất các thuộc tính giao dịch:
  - ID, Date, Amount, Description.
  - Wallet ID.
  - Income Source ID hoặc Expense Category ID.
- Thiết kế thông tin cho Recurring Transactions:
  - Transaction Type.
  - Start Date và optional End Date.
  - Monthly recurrence.
  - Thông tin cần thiết để tránh tạo giao dịch trùng.
- Tạo interface cho:
  - `include/managers/TransactionManager.h`
  - `include/managers/RecurringTransactionManager.h`
- Khai báo các thao tác chính:
  - Add, View, Edit, Delete Transactions.
  - Create, View, Edit, Delete Recurring Transactions.
- Thống nhất với TV2 (Khôi) cách kiểm tra Wallet ID và cập nhật số dư.
- Thống nhất với TV1 (Vy) kiểu dữ liệu tiền, Date và cách quản lý ID.

**Deliverable:**

- Transaction và Recurring models.
- Interface của các Manager.
- Quy tắc liên kết giao dịch với Master Data.

> Chưa cần implement Recurring Generation hoặc Binary Persistence trong Week 1.

### 🟠 TV4 (Tân) — Statistics & Dashboard

**Branch:** `feat/statistics-design`

**Thời gian dự kiến:** ~5–7 giờ

- Xác định các báo cáo cần thực hiện theo yêu cầu đề:
  - Tổng Income, Expense và Net Balance theo khoảng thời gian.
  - Income/Expense theo Wallet.
  - Annual Overview cho một hoặc nhiều năm.
  - Income by Source.
  - Expense by Category.
- Thiết kế Dashboard:
  - Total Balance.
  - Balance của từng Wallet.
  - Vị trí hiển thị khi khởi động ứng dụng.
- Tạo skeleton:
  - `include/statistics/StatisticsManager.h`
  - `include/statistics/NumberFormatter.h`
  - `include/ui/Dashboard.h`
  - `include/ui/StatisticsMenu.h`
- Khai báo các public methods cần thiết cho Statistics và Dashboard.
- Thống nhất với TV2 (Khôi) và TV3 (Uyên) cách truy cập dữ liệu Wallet và Transactions.
- Chuẩn bị dữ liệu mẫu và kết quả tính toán dự kiến để kiểm thử các báo cáo.
- Thống nhất với TV1 (Vy) cách điều hướng từ Dashboard sang Main Menu và quay lại.

**Deliverable:**

- Statistics/Dashboard skeleton.
- Danh sách báo cáo theo yêu cầu.
- Test cases mẫu với kết quả tính toán dự kiến.

> Chưa cần implement toàn bộ thuật toán thống kê trong Week 1.

## Thứ tự phối hợp trong tuần

1.  **Đầu tuần (Thứ Hai – Thứ Ba):** TV1 (Vy) setup CMake và CLI skeleton; cả nhóm thống nhất kiểu dữ liệu và shared interfaces.
2. **Giữa tuần (Thứ Tư – Thứ Năm):** TV2 (Khôi), TV3 (Uyên), TV4 (Tân) hoàn thành các task được giao, tạo Pull Request; TV1 review và merge các PR hợp lệ.
3. **Thứ Sáu:** Cả nhóm build, kiểm thử, demo checkpoint; TV1 tổng hợp tiến độ, Git Commit Log, AI Usage Declaration và nộp Weekly Report.

> Nếu một thành viên cần interface của module khác nhưng chưa hoàn thành, phải trao đổi trước, không tự tạo interface riêng gây trùng lặp.

## Checklist cuối Week 1

- Project build được bằng CMake trên macOS/Windows.
- `FinanceManager` hiển thị menu CLI cơ bản.
- `DynamicArray` và `Date` có chức năng cơ bản.
- Data models và Manager interfaces của TV2, TV3 hoàn thành.
- Statistics và Dashboard skeleton hoàn thành.
- Thống nhất ID, kiểu dữ liệu tiền, shared interfaces và Binary format sơ bộ.
- Mỗi thành viên có branch, commit và Pull Request riêng.
- TV1 merge các PR hợp lệ; cả nhóm build và demo thành công.
