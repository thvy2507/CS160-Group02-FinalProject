# **Phân Công Công Việc — Sprint 2: Core Management**

> **Mục tiêu:** Implement các chức năng quản lý dữ liệu chính trong RAM, kết nối Master Data với Income/Expense Transactions, cập nhật chính xác số dư Wallet và bổ sung thống kê cơ bản theo khoảng thời gian.
>
> **Demo checkpoint:** Chạy ứng dụng → tạo Wallet, Income Source và Expense Category → thêm Income/Expense → kiểm tra Wallet Balance → xem tổng Income, Expense và Net Balance theo khoảng ngày mà không gặp lỗi.

## Quy trình Git của cả team

Mỗi người **tiếp tục dùng branch feature/module từ Sprint 1**, đồng bộ code mới nhất từ `main` trước khi implement Sprint 2. Không commit code trực tiếp vào `main`:

```tex
main          (ổn định, merge cuối tuần)
├── feat/core-setup               ← Vy
├── feat/master-data-models       ← Khôi
├── feat/transaction-models       ← Uyên
├── feat/statistics-design        ← Tân
```

**Các bước cho mỗi người:**

```bash
# 1. Kiểm tra và đảm bảo các thay đổi đang làm đã được lưu an toàn
git status
# Nếu còn file chưa commit, hãy commit hoặc stash trước khi chuyển branch.

# 2. Cập nhật main
git switch main
git pull --ff-only origin main

# 3. Quay lại branch feature đã dùng ở Sprint 1
git switch feat/ten-branch-cu

# 4. Đưa code mới nhất từ main vào branch feature hiện tại
git merge main

# 5. Implement Sprint 2, test và commit những file mình phụ trách
git add <cac-file-da-chinh-sua>
git commit -m "[TVx] feat: short description"
git push origin feat/ten-branch-cu

# 6. Hoàn thành → tạo Pull Request vào main trên GitHub
# 7. Vy review, kiểm tra build và merge PR vào main
```

---

## Những gì đã chuẩn bị

| Hạng mục                                                     | Trạng thái                                 |
| ------------------------------------------------------------ | ------------------------------------------ |
| C++17, CMake và CLI skeleton                                 | Đã hoàn thành trong Sprint 1               |
| `DynamicArray<T>` và `Date`                                  | Đã implement và có core tests              |
| Wallet, IncomeSource, ExpenseCategory models và Manager interfaces | Đã merge — PR #7                           |
| Transaction, Income, Expense, RecurringTransaction models và Manager interfaces | Đã merge — PR #8                           |
| Statistics và Dashboard skeleton, test cases mẫu             | Đã merge — PR #5                           |
| Quy ước money `long long`, object ID `int`, shared interfaces | Đã thống nhất ở mức Sprint 1               |
| Binary records và Name Pool                                  | Đã thiết kế sơ bộ, chưa implement File I/O |
| Clean build trên `main` sau lần merge cuối, xác nhận macOS/Windows | macOS: **PASS** — Windows: **PASS**        |

---

## Việc cần làm — Từng thành viên

### 🔴 TV1 (Vy) — Leader / Core & Integration

**Branch:** `feat/core-setup`

**Thời gian dự kiến:** ~8–11 giờ

- Hoàn thiện `src/core/FinanceManager.cpp` và các file liên quan:
  - Thay thông báo `This module is not implemented yet` đối với những chức năng đã có implementation.
  - Bổ sung menu và xử lý input cơ bản cho Wallet, Income Source, Expense Category, Income và Expense.
  - Chọn ID, ngày tháng và số tiền; xử lý input không hợp lệ mà không làm chương trình crash.
  - Liên kết thống kê theo khoảng ngày khi TV4 hoàn thành.
- Cập nhật `CMakeLists.txt` để compile và link các file `.cpp` mới của TV2 và TV3.
- Điều phối thống nhất interface giữa các module:
  - TV2 quản lý dữ liệu Wallet và thay đổi balance.
  - TV3 kiểm tra ID trước khi ghi giao dịch và cập nhật balance đúng một lần.
  - TV4 chỉ đọc Transaction data để tính Statistics.
- Chốt với TV4 quy tắc Dashboard và Statistics Menu:
  - Dashboard được dự kiến hiển thị lúc khởi động, sau khi dữ liệu đã sẵn sàng.
  - `0 (Back)` quay về Main Menu; `0 (Exit)` ở Main Menu kết thúc chương trình.
  - Thống nhất trong tài liệu; chưa cần render Dashboard hoàn chỉnh trong Sprint 2.
- Review, build/test và merge Pull Requests; tích hợp các chức năng đã hoàn thiện vào CLI.

**Deliverable:**

- `FinanceManager` gọi được các chức năng Master Data, Income/Expense và thống kê cơ bản đã implement.
- CMake build và CLI demo hoạt động với các module mới.
- Ghi nhận kết quả integration Sprint 2.

### 🟢 TV2 (Khôi) — Master Data & Wallets

**Branch:** `feat/master-data-models`

**Thời gian dự kiến:** ~12–15 giờ

- Implement các model đã khai báo trong Sprint 1:
  - `src/models/Wallet.cpp`
  - `src/models/IncomeSource.cpp`
  - `src/models/ExpenseCategory.cpp`
  - Constructor, getter/setter, validation và các phương thức dùng trong RAM.
- Implement các Manager:
  - `src/managers/WalletManager.cpp`
  - `src/managers/IncomeSourceManager.cpp`
  - `src/managers/ExpenseCategoryManager.cpp`
- Hoàn thiện CRUD trong RAM cho Wallet, Income Source và Expense Category:
  - Add, View, Edit và Delete.
  - Tìm kiếm theo ID; sử dụng `DynamicArray<T>`.
  - Quản lý ID ổn định, bắt đầu từ 1 và không tái sử dụng ID đã xóa.
  - Phân biệt object còn hoạt động và object đã bị xóa.
- Implement các thao tác Wallet Balance:
  - `deposit`, `withdraw`, `canWithdraw`, `adjustBalance`.
  - `getWalletBalance` và `getTotalBalance`.
  - Kiểm tra số tiền hợp lệ và xử lý trường hợp không đủ số dư theo quy tắc đã thống nhất.
- Phối hợp TV3:
  - Cung cấp cách kiểm tra Wallet ID, Income Source ID và Expense Category ID.
  - Thống nhất thao tác cộng/trừ balance khi thêm Income/Expense.
- Viết test dữ liệu trong RAM.

**Deliverable:**

- CRUD hoạt động trong RAM cho ba nhóm Master Data.
- Wallet Balance được cập nhật chính xác qua các method đã khai báo.
- Có test cases cho CRUD, ID, validation và balance.

> Chưa cần implement Binary Persistence, Name Pool hoặc đọc/ghi file trong Sprint 2.

### 🔵 TV3 (Uyên) — Transactions & Recurring

**Branch:** `feat/transaction-models`

**Thời gian dự kiến:** ~11–14 giờ

- Implement các Transaction models cần dùng trong Sprint 2:
  - `src/models/Transaction.cpp`
  - `src/models/Income.cpp`
  - `src/models/Expense.cpp`
  - Constructor, getter/setter, virtual destructor và `getType()`.
- Implement phần cần thiết của `src/managers/TransactionManager.cpp`:
  - Thêm Income và Expense.
  - Hiển thị danh sách giao dịch và tìm giao dịch theo ID.
  - Cấp phát ID giao dịch, quản lý `DynamicArray<Transaction*>` và giải phóng bộ nhớ trong destructor.
- Kiểm tra dữ liệu khi tạo giao dịch:
  - Amount phải lớn hơn 0; sử dụng `long long` VND.
  - Wallet ID và Source/Category ID phải hợp lệ, còn hoạt động.
  - Date hợp lệ qua `Date` chung của team.
- Phối hợp TV2 cập nhật Wallet Balance:
  - Income làm tăng số dư đúng một lần.
  - Expense làm giảm số dư đúng một lần sau khi kiểm tra khả năng chi trả.
  - Giao dịch không hợp lệ không được thêm vào danh sách và không được thay đổi số dư.
- Tạo test cases:
  - Thêm Income/Expense cho một hoặc nhiều Wallet.
  - Invalid ID, amount bằng 0/âm, không đủ tiền.
  - Kiểm tra balance trước và sau giao dịch.
- Phối hợp TV1 kết nối chức năng Income/Expense vào CLI và TV4 đọc danh sách giao dịch.

**Deliverable:**

- Có thể thêm và xem Income/Expense trong RAM.
- Số dư Wallet được đồng bộ chính xác với giao dịch mới.
- `TransactionManager` hoạt động với các Manager của TV2 và có test cases.

> Recurring Generation, Binary Persistence và hoàn thiện các thao tác Edit/Delete Transaction được thực hiện ở các giai đoạn tiếp theo theo `WEEKLY_PLAN.md`; không đánh dấu đã hoàn thành trong Sprint 2 nếu chưa implement/test.

### 🟠 TV4 (Tân) — Statistics & Dashboard

**Branch:** `feat/statistics-design`

**Thời gian dự kiến:** ~7–10 giờ

- Implement thống kê cơ bản trong `StatisticsManager`:
  - Tính tổng Income theo khoảng thời gian.
  - Tính tổng Expense theo khoảng thời gian.
  - Tính Net Balance (`totalIncome - totalExpense`) theo khoảng thời gian.
- Sử dụng interface đã thống nhất với TV3:
  - Đọc dữ liệu từ `TransactionManager::getTransactionCount()` và `getTransactionAt()`.
  - Phân biệt Income và Expense thông qua Transaction Type.
  - Không sửa dữ liệu giao dịch hoặc Wallet Balance.
- Xử lý các trường hợp thống kê:
  - Khoảng ngày hợp lệ, bao gồm cả ngày bắt đầu và ngày kết thúc.
  - Không có giao dịch phù hợp → kết quả bằng 0.
  - Khoảng ngày không hợp lệ → báo lỗi, không ghi đè kết quả đầu ra.
- Bổ sung test cases trong `tests/test_statistics.cpp`:
  - Nhiều khoản thu/chi trong cùng khoảng ngày.
  - Giao dịch ngoài khoảng ngày.
  - Trường hợp không có dữ liệu và khoảng ngày sai.
- Phối hợp TV1 để truy cập báo cáo qua Statistics Menu.
- Chốt và ghi lại quy tắc Dashboard/Main Menu navigation; giữ các report chưa implement ở trạng thái chưa khả dụng.

**Deliverable:**

- Báo cáo Income, Expense và Net Balance theo khoảng ngày hoạt động với Transaction data thật.
- Có sample data, expected results và test cases cho báo cáo cơ bản.
- Dashboard và Statistics Menu có quy tắc điều hướng được thống nhất với TV1.

> Chưa cần implement Annual Overview, Income/Expense Breakdown, Wallet-based Statistics hoặc Dashboard đầy đủ trong Sprint 2.

---

## Thứ tự phối hợp trong tuần

1. **Đầu tuần (Thứ Hai – Thứ Ba):** TV1 xác nhận các interface cần dùng; TV2 triển khai Master Data và Balance; TV3 bắt đầu Transaction models; TV4 chuẩn bị test cases cho thống kê theo ngày.
2. **Giữa tuần (Thứ Tư – Thứ Năm):** TV2 cung cấp các method tìm ID/cập nhật balance; TV3 tích hợp Income/Expense; TV4 triển khai `calculateSummary`; TV1 cập nhật CLI và CMake, review Pull Requests.
3. **Thứ Sáu:** Cả nhóm pull `main`, build, kiểm thử luồng tạo Wallet → thêm Income/Expense → xem balance → xem thống kê; TV1 tổng hợp Git Commit Log, AI Usage Declaration, evidence và Weekly Report.

> Ưu tiên hoàn thành **TV2 Wallet/Manager → TV3 Income/Expense → TV4 Statistics → TV1 CLI Integration**. Các thành viên vẫn code song song trên interface đã thống nhất, không chờ nhau hoàn tất toàn bộ mới bắt đầu.

---

## Checklist cuối Sprint 2

- `Wallet`, `IncomeSource`, `ExpenseCategory` có CRUD hoạt động trong RAM.
- ID quản lý nhất quán; Wallet Balance và validation hoạt động đúng.
- Thêm và xem được Income/Expense; giao dịch liên kết đúng Wallet/Source/Category.
- Thêm Income/Expense cập nhật đúng số dư; thao tác không hợp lệ không làm sai dữ liệu.
- Statistics tính đúng tổng Income, Expense và Net Balance theo khoảng ngày.
- `FinanceManager` điều hướng đến các chức năng đã implement; chương trình không crash với input sai cơ bản.
- CMake compile/link toàn bộ source cần thiết; mỗi module có test cases và kết quả kiểm tra.
- Mỗi thành viên cập nhật branch feature cũ bằng `main`, push các commit Sprint 2 và tạo Pull Request mới; TV1 review và merge những PR hợp lệ.
- Hoàn thành demo checkpoint, Weekly Report, AI Usage Declaration và Git Commit Log trước hạn báo cáo.
