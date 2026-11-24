# **Phân Công Công Việc — Sprint 3: Persistence & Recurring Transactions**

> **Mục tiêu:** Triển khai Binary Persistence cơ bản bằng fixed-size records và Name Pool, bổ sung Recurring Transactions hằng tháng trong RAM, và hoàn thiện Wallet-based Statistics.
>
> **Demo checkpoint:** Binary save/load hoạt động với dữ liệu mẫu; Master Data được khôi phục chính xác; recurring transactions hoạt động đúng lịch trong RAM và không tạo trùng; Wallet-based Statistics cho kết quả chính xác.

## Quy trình Git của cả team

Mỗi người tiếp tục sử dụng feature branch hiện tại, đồng bộ code mới nhất từ `main` trước khi implement Sprint 3. Không push trực tiếp vào `main`.

```text
main
├── feat/core-setup                  ← Vy
├── feat/master-data-models          ← Khôi
├── feat/transaction-models          ← Uyên
└── feat/statistics-design           ← Tân
```

**Các bước cho mỗi người:**

```bash
# 1. Check working tree
git status

# 2. Update main
git switch main
git pull --ff-only origin main

# 3. Switch to the existing feature branch
git switch feat/your-branch

# 4. Merge the latest main
git merge main

# 5. Implement, build and test
git add <specific-files>
git commit -m "[TVx] feat: short description"
git push origin feat/your-branch

# 6. Create a Pull Request into main
# 7. TV1 reviews and merges approved PRs
```

Không commit generated Binary files hoặc build artifacts. Tuân thủ `CODING_RULES.md`.

---

## Những gì đã chuẩn bị

| Hạng mục                             | Trạng thái                         |
| ------------------------------------ | ---------------------------------- |
| C++17, CMake và CLI                  | Hoàn thành Sprint 1–2              |
| Shared `DynamicArray<T>` và `Date`   | Đã implement                       |
| Master Data CRUD và Balance          | Đã implement trong RAM             |
| Income/Expense Transactions          | Add/View/Search hoạt động          |
| Date-range Statistics                | Đã implement và test               |
| Master Data Binary Record structures | Đã có struct và conversion methods |
| Master Data save/load interfaces     | Đã khai báo, chưa implement        |
| Transaction Binary Persistence       | Chưa có interface                  |
| Recurring models và Manager          | Header only                        |
| Recurring Generation interface       | Chưa khai báo                      |
| Wallet-based Statistics              | Đã khai báo, chưa implement        |
| Binary File I/O và Name Pool         | Chưa implement                     |

**Baseline:** `main` tại commit `300ee87`.

---

## Việc cần làm — Từng thành viên

### 🔴 TV1 (Vy) — Leader / Core & Integration

**Branch:** `feat/core-setup`

**Thời gian dự kiến:** ~10–13 giờ

- Thiết kế và thống nhất Binary Persistence format trước khi implement:
  - Phân biệt in-memory record structures và on-disk binary layout.
  - Quy định field sizes, field order, byte order và record size trên file.
  - Không phụ thuộc vào compiler-specific padding hoặc `sizeof(struct)` để xác định file format.
  - Thống nhất Name Pool offsets/lengths, string encoding và giới hạn dữ liệu.
  - Thống nhất file layout, file validation và cách xử lý file thiếu hoặc bị lỗi.
- Implement và test basic Binary File I/O utilities theo format đã thống nhất.
- Phối hợp TV2/TV3 thống nhất save/load contracts và trách nhiệm serialization của từng module.
- Phối hợp TV3 thống nhất generation interface giữa `RecurringTransactionManager` và `TransactionManager`, bảo đảm Wallet Balance chỉ được cập nhật một lần.
- Điều phối thứ tự save/load; không replay balance updates khi khôi phục transactions.
- Cập nhật CMake, review PRs, xử lý compatibility issues và thực hiện integration tests.

**Deliverable:**

- Binary format và Name Pool conventions được document rõ ràng.
- Basic Binary I/O utilities hoạt động với dữ liệu mẫu.
- Shared interfaces và generation contracts được thống nhất.
- Có kết quả kiểm thử Binary I/O và Sprint 3 integration.

### 🟢 TV2 (Khôi) — Master Data Persistence

**Branch:** `feat/master-data-models`

**Thời gian dự kiến:** ~8–11 giờ

- Implement `saveToBinary()` và `loadFromBinary()` cho:
  - `WalletManager`
  - `IncomeSourceManager`
  - `ExpenseCategoryManager`
- Sử dụng fixed-size records và Name Pool theo Binary format do TV1 thống nhất.
- Khôi phục chính xác IDs, next ID, names, soft-delete flags, initial balance và current balance.
- Validate records và Name Pool references trước khi khôi phục dữ liệu.
- Load vào trạng thái tạm; chỉ thay thế dữ liệu hiện tại sau khi validate thành công.
- Kiểm tra file thiếu, file bị cắt ngắn, invalid records và offset/length ngoài giới hạn.
- Test save/load round-trip, deleted records và ID stability.

**Deliverable:**

- Master Data save/load Binary hoạt động.
- Name Pool được sử dụng đúng theo format.
- Có test cases cho data restoration và invalid files.

### 🔵 TV3 (Uyên) — Recurring Transactions & Persistence

**Branch:** `feat/transaction-implementation`

**Thời gian dự kiến:** ~12–16 giờ

**Ưu tiên 1 — Recurring Generation & Testing**

- Implement `RecurringTransaction.cpp` và `RecurringTransactionManager.cpp` dựa trên các header hiện tại.
- Phối hợp TV1 bổ sung generation interface cho phép `RecurringTransactionManager` sử dụng `TransactionManager`.
- Tạo giao dịch thật thông qua `TransactionManager::addIncome()` và `addExpense()`:
  - Tái sử dụng validation và balance operations hiện có.
  - Không gọi thêm `deposit()` hoặc `withdraw()` từ recurring generation.
  - Chỉ cập nhật generation state sau khi transaction được tạo thành công.
- Implement monthly recurring generation:
  - Start Date và optional End Date.
  - Active/inactive status.
  - Day of month; tháng ngắn hơn sử dụng ngày cuối tháng.
  - Chống generate trùng bằng generation state.
- Test recurring dates, leap years, month boundaries, invalid references, insufficient balance và repeated generation.
- Bảo đảm failed generation không tạo transaction sai hoặc làm thay đổi balance ngoài dự kiến.

**Ưu tiên 2 — Basic Binary Persistence**

- Thống nhất Transaction/Recurring record layouts với TV1.
- Bổ sung save/load interfaces theo format đã thống nhất.
- Bắt đầu đọc/ghi Binary cho dữ liệu mẫu, bao gồm transaction IDs và recurring generation state.
- Không replay Wallet Balance updates khi load transactions.
- Ghi rõ những phần persistence đã implement, test hoặc còn pending.

**Deliverable:**

- Monthly recurring generation hoạt động trong RAM.
- Giao dịch được tạo thông qua `TransactionManager`, không cập nhật balance trùng lặp.
- Có test cases cho recurring generation và duplicate prevention.
- Bước đầu triển khai Binary Persistence cho Transactions và Recurring; ghi nhận kết quả implementation và testing thực tế, không bắt buộc hoàn thiện toàn bộ trong Sprint 3.

> Recurring generation và tests là ưu tiên chính. Không yêu cầu hoàn thiện toàn bộ Transaction/Recurring Persistence trong Sprint 3. Full persistence, cross-session duplicate prevention và các trường hợp missed months được tiếp tục hoàn thiện trong Sprint 4.

### 🟠 TV4 (Tân) — Wallet-based Statistics

**Branch:** `feat/statistics-design`

**Thời gian dự kiến:** ~8–11 giờ

- Implement `StatisticsManager::calculateWalletSummary()` theo interface hiện tại.
- Tính Income, Expense và Net Balance theo Wallet ID và date range, bao gồm cả hai ngày giới hạn.
- Validate Wallet ID, active status, date range và transaction data.
- Sử dụng read-only interfaces của TransactionManager và Wallet.
- Không thay đổi Transaction hoặc Wallet state.
- Kiểm tra integer overflow trong quá trình tính toán.
- Test nhiều Wallet, invalid IDs, invalid date ranges, empty results và failed calculations.
- Phối hợp TV1 tích hợp báo cáo vào Statistics Menu.

**Deliverable:**

- Wallet-based Statistics hoạt động chính xác.
- Có test cases và expected results.
- Interface tương thích với FinanceManager và Statistics Menu.

---

## Thứ tự phối hợp trong tuần

### Thứ Ba — 24/11: Interface & Format Agreement

- TV1 thống nhất Binary record layouts, field sizes, byte order, Name Pool conventions và file validation rules.
- TV2 xác nhận Master Data records và save/load interfaces.
- TV1 và TV3 thống nhất dependency giữa RecurringTransactionManager và TransactionManager.
- TV3 bắt đầu recurring implementation.
- TV4 bắt đầu Wallet-based Statistics bằng interfaces hiện có.

**Checkpoint:** Shared Binary format và generation interface được thống nhất trước khi các module triển khai phần phụ thuộc.

### Thứ Tư — 25/11: Core Implementation

- TV1 implement và test basic Binary I/O utilities.
- TV2 triển khai Master Data save/load và Name Pool.
- TV3 tập trung recurring generation trong RAM và unit tests.
- TV4 hoàn thiện Wallet-based Statistics và unit tests.

**Checkpoint:** Basic Binary I/O và các chức năng độc lập có kết quả kiểm thử ban đầu.

### Thứ Năm — 26/11: Testing & Integration

- TV1 review PRs và kiểm tra Binary format compatibility.
- TV2 hoàn thiện Master Data round-trip tests.
- TV3 hoàn thiện recurring generation tests, sau đó triển khai Binary Persistence cơ bản theo thời gian còn lại.
- TV4 hoàn thiện PR và hỗ trợ regression testing.
- Merge các PR đạt yêu cầu theo dependency order.

**Checkpoint:** Các chức năng ưu tiên sẵn sàng cho demo; những phần persistence chưa hoàn thiện được ghi nhận rõ ràng.

### Thứ Sáu — 27/11: Demo & Documentation

- Cả nhóm đồng bộ `main` và clean build.
- Test Binary save/load với dữ liệu mẫu.
- Kiểm tra recurring generation và duplicate prevention.
- Kiểm tra Wallet Balance sau generation và sau load đối với các module đã có persistence.
- Chạy Wallet-based Statistics với nhiều Wallet.
- Tổng hợp Git Commit Log, AI Usage Declaration, evidence và Weekly Report.
- Ghi rõ các phần chưa hoàn thành hoặc chưa được kiểm thử.

---

## Integration Dependencies

```text
Persistence:

TV1: Binary Format & I/O Contracts
  ├── TV2: Master Data Persistence
  └── TV3: Basic Transaction/Recurring Persistence

Recurring Generation:

RecurringTransactionManager
  └── TransactionManager
        ├── WalletManager
        ├── IncomeSourceManager
        └── ExpenseCategoryManager

Statistics:

StatisticsManager
  ├── TransactionManager (read-only)
  └── Wallet Data (read-only)
```

**Integration order:**

1. Thống nhất Binary format và recurring generation interface.
2. Hoàn thành và tích hợp basic Binary I/O utilities.
3. Tích hợp Master Data Persistence.
4. Tích hợp Recurring Generation và Wallet-based Statistics khi các module đã pass tests.
5. Tích hợp Transaction/Recurring Persistence cơ bản đối với các phần đã sẵn sàng.
6. Thực hiện regression tests và Friday demo.

**Data loading order:**

```
Master Data
    ↓
Transactions
    ↓
Recurring State
    ↓
Application Ready
```

Master Data được load trước để các module còn lại có thể khôi phục references. Transaction loading không được cộng/trừ lại Wallet Balance.

Recurring Generation và Wallet-based Statistics không phụ thuộc vào việc hoàn thành Binary Persistence. Các thành viên triển khai song song và merge những PR độc lập đã đạt yêu cầu.

---

## Checklist cuối Sprint 3

- [ ] Binary file format được document, không phụ thuộc compiler padding.
- [ ] Field sizes, byte order, record sizes và Name Pool conventions được thống nhất.
- [ ] Basic Binary File I/O hoạt động với dữ liệu mẫu.
- [ ] Master Data save/load khôi phục đúng IDs, names, balances và soft-delete flags.
- [ ] Failed load không phá hủy dữ liệu đang có trong RAM của module.
- [ ] Recurring generation sử dụng TransactionManager để tạo Income/Expense.
- [ ] Monthly recurring transactions hoạt động đúng lịch trong RAM và không tạo trùng.
- [ ] Wallet Balance chỉ được cập nhật một lần cho mỗi transaction được tạo thành công.
- [ ] Recurring generation có tests cho các trường hợp hợp lệ và không hợp lệ.
- [ ] Ghi nhận tiến độ triển khai và kết quả kiểm thử bước đầu của Transaction/Recurring Binary Persistence.
- [ ] Wallet-based Statistics hoạt động và có tests.
- [ ] CMake build và regression tests được chạy sau integration.
- [ ] PRs được review và merge theo đúng workflow.
- [ ] Demo checkpoint và Weekly Report được hoàn thành.

> **Scope boundary:** Full Dashboard, Annual Overview, Income/Expense Breakdown, Transaction Edit/Delete, full persistence integration, random-access updates và persistence hardening thuộc các giai đoạn tiếp theo. Không đánh dấu hoàn thành những chức năng chưa implement hoặc test.
