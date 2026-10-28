# **General Rules — Coding & Git**

> These are the rules of the team. If anyone violates them → TV1 has the right to request a refactor before merging.

## 1. Git Workflow

### Branches

```
main       ← stable, integrated code; only TV1 can merge PRs
feat/...   ← new feature development
fix/...    ← bug fixes
docs/...   ← documentation updates
```

**Process:**

```text
1. Start a new task:
   git switch main
   git pull origin main
   git switch -c feat/wallet-management

2. Finish a task → commit:
   git add .
   git commit -m "[TV2] feat: implement wallet management"

3. Push to remote:
   git push -u origin feat/wallet-management

4. Create a Pull Request into main on GitHub
   → Request a review from TV1
   → Other members may also review and leave comments
   → Fix any issues identified during review

5. End of week (Sunday):
   TV1 reviews all ready Pull Requests
   Whole team pulls main and rebuilds to confirm
```

### Commit Message Format

```text
[TVx] <type>: <short description>

Good examples:
[TV2] feat: implement wallet CRUD
[TV3] fix: prevent duplicate recurring transactions
[TV1] refactor: simplify date validation
[TV4] test: add annual statistics tests
[TV1] docs: update weekly plan
[TV1] build: configure CMake

Bad examples (do not use):
fix stuff
update code
TV3 done
final
wip
```

### Commit Rules

- **All commit messages must be written in English.**
- **Commit frequently**, after each small unit of work — do not bundle 8 hours of work into 1 commit.
- **Do not commit code that does not compile**
- **Do not commit build files** (`/build`, `.exe`, `.o`) — `.gitignore` is already set up.
- **Do not commit generated data files** (`data/*.bin`).
- **Do not commit temporary files** (`debug.txt`, personal notes, unused test files).
- Keep commits focused and meaningful.

---

## 2. C++ Coding Conventions

### Language Rule

- **English Only**: All code identifiers (variable names, class names, functions, etc.), source comments, and project documentation must be written in English.

### Naming

```cpp
// Class → PascalCase
class WalletManager { };
class TransactionManager { };

// Method → camelCase
bool addWallet(const std::string& name);
bool loadFromFile(const std::string& path);

// Variable → camelCase
double walletBalance = 0.0;
int transactionCount = 0;

// Private member → m_ prefix
class Wallet {
private:
    int m_id;
    std::string m_name;
    double m_balance;
};

// Constant & enum value → UPPER_SNAKE_CASE
const int INITIAL_CAPACITY = 10;

enum class TransactionType { INCOME , EXPENSE };
// use scoped enum (enum class), do not use plain enum


// File → class name (1-to-1)
// WalletManager → WalletManager.h + WalletManager.cpp
```

### Class Structure — Layout Order

```cpp
class WalletManager {
public:
    // 1. Constructor / Destructor
    WalletManager();
    ~WalletManager();

    // 2. Public methods (alphabetical)
    bool addWallet(const std::string& name);
    bool deleteWallet(int id);
    bool updateWallet(int id, const std::string& name);

    // 3. Getters / Setters
    double getBalance(int id) const;
    int getWalletCount() const;

private:
    // 4. Private methods
    int findWalletIndex(int id) const;
    bool isValidWalletId(int id) const;

    // 5. Private members
    DynamicArray<Wallet> m_wallets;
    int m_nextId;
};
```

### Pointers & Memory

```cpp
// ALLOWED: manual dynamic memory allocation
int* numbers = new int[10];

// Release memory using delete[]
delete[] numbers;
numbers = nullptr;

// Use the custom DynamicArray<T> for collections
DynamicArray<Wallet> wallets;

// std::string is allowed
std::string walletName = "Cash";
```

### Functions

```cpp
// Recommended maximum: approximately 40–50 lines
// If longer → consider splitting into helper functions

// Methods that do not modify object state → const
int getWalletCount() const;
double getBalance() const;

// Large parameters → pass by const reference
bool addWallet(const std::string& name);
bool loadData(const std::string& path);

// Return bool for operations that can fail
bool deleteWallet(int id);
bool saveData(const std::string& path); // false if file doesn't exist
```

### Do Not Use

```cpp
// Do not use using namespace std; in headers — namespace pollution
// BAD in header:
using namespace std;

// OK in .cpp (and only there):
using namespace std;  // acceptable, but prefer explicit std::

// Do not use magic numbers
// BAD:
if (month > 12) return false;

// GOOD:
const int MONTHS_PER_YEAR = 12;
if (month > MONTHS_PER_YEAR) return false;

// Do not leave dead code in Pull Requests
// BAD:
// void oldSaveData() { ... }
```

---

## 3. Required Header Comments

Each `.cpp` and `.h` file must have a header comment like this:

```cpp
/**
 * @file WalletManager.cpp
 * @author TV2
 * @brief Manages wallet CRUD operations and balances
 * @note Week 2 - uses the shared DynamicArray template
 */
```

**Rules:**

- Use the correct file name in `@file`.
- Use the responsible member's identifier in `@author`.
- Briefly describe the file's purpose in `@brief`.
- Use `@note` when additional information is necessary.
- Update comments when the implementation changes significantly.

---

## 4. Pull Request Checklist

Before creating a PR, check yourself:

- Code compiles successfully with CMake.
- No known memory leaks or invalid memory access.
- No dead code or unnecessary commented-out code.
- No unnecessary debug output.
- Implemented features have been manually tested.
- Branch name and commit message follow the required format.
- No unrelated files or generated data are included.
- Shared interfaces remain compatible.

**Reviewer check:**

- Logic follows the agreed module design.
- Naming and coding conventions are followed.
- No prohibited STL containers are used.
- Dynamic memory is managed correctly.
- Build is not broken.
- Related modules still work correctly.

---

## 5. Team Meetings Rules

- **Weekly Sync Meeting (~30 minutes):** Each member reports progress and tasks for the week.
- **Mid-week Sync (~15 minutes):** Discuss blockers, dependencies, and interface changes.
- **Sunday Demo Checkpoint:** Build and run the application, demonstrate completed features, and verify the weekly checklist.
- **Communication Channels:** Use the group's agreed communication platform for updates and technical discussions.
- **GitHub Pull Requests:** Use PR comments for code reviews and implementation feedback.

---

## 6. Pre-submission Checklist

In **Week 5**, TV1 coordinates the final checklist with all team members:

- Clean build succeeds on macOS and Windows.
- All required features work correctly.
- Wallet, income source, and expense category CRUD operations work.
- Income and expense transactions update wallet balances correctly.
- Monthly recurring transactions are generated correctly without duplicates.
- Data persists correctly in `data/*.bin` across sessions.
- Dashboard displays total balance and individual wallet balances.
- Statistics work for date ranges, wallets, and one or multiple years.
- Income and expense breakdowns produce correct results.
- Invalid inputs and edge cases are handled.
- No known memory leaks or crashes remain.
- `README.md` and required project documentation are complete.
- Individual GitHub commits and contributions are recorded.
- Project report and work-division documentation are complete.
- Demo videos are recorded.
- Final submission files are checked against the project requirements.
- Project is submitted in the required format before the deadline.
