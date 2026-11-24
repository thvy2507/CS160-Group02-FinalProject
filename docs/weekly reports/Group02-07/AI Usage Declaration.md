# AI Usage Declaration — Week 07

## 1. Declaration

**We used AI tools for this project during the reporting period.**

**25125049 - Trần Hồng Vy (TV1)**

I used ChatGPT (OpenAI) to assist with Core implementation, CLI integration, code review, debugging, regression testing, Git management, and project documentation.

**26125041 - Lê Thiện Khôi (TV2)**

I used Gemini (Google) to assist with Master Data CRUD implementation, ID management, wallet balance operations, and test development.

**26125089 - Dương Thị Nhã Uyên (TV3)**

I used Claude (Anthropic) to assist with transaction implementation, code explanation, data validation, test preparation, and Git troubleshooting.

**26125070 - Trần Nhật Tân (TV4)**

I used ChatGPT (OpenAI) to assist with date-range statistics implementation, statistics testing, menu development, integration debugging, and Git/GitHub operations.

All members reviewed and adapted AI-assisted suggestions and took responsibility for their submitted work.

---

## 2. AI Tool Information

**25125049 - Trần Hồng Vy (TV1)**

| Item | Details |
|---|---|
| AI Tool      | ChatGPT                                                      |
| Provider     | OpenAI                                                       |
| Platform     | ChatGPT                                                      |
| Model        | GPT-6                                                        |
| Usage Period | 14/11/2026 – 20/11/2026           |
| Purposes     | Core implementation, CLI integration, code review, compatibility fixes, input validation, overflow protection, automated testing, and documentation. |
| Evidence     | ChatGPT conversation history and summarized prompt logs provided in Section 3. |

**26125041 - Lê Thiện Khôi (TV2)**

| Item         | Details                                                      |
| ------------ | ------------------------------------------------------------ |
| AI Tool      | Gemini                                                       |
| Provider     | Google                                                       |
| Platform     | Gemini Web App                                               |
| Model        | Gemini                                                       |
| Usage Period | 14/11/2026 – 20/11/2026                                      |
| Purposes     | In-memory Master Data CRUD implementation, ID management, soft deletion, wallet balance management, and test development. |
| Evidence     | Gemini conversation history and summarized prompt logs provided in Section 3. |

**26125089 - Dương Thị Nhã Uyên (TV3)**

| Item         | Details                                                      |
| ------------ | ------------------------------------------------------------ |
| AI Tool      | Claude                                                       |
| Provider     | Anthropic                                                    |
| Platform     | Claude (web/mobile chat)                                     |
| Model        | Claude Sonnet 5.5                                            |
| Usage Period | 17/11/2026 – 20/11/2026                                      |
| Purposes     | Transaction implementation guidance, code explanation, transaction validation, wallet balance updates, test planning, and Git troubleshooting. |
| Evidence     | Claude conversation history and summarized prompt logs provided in Section 3. |

---

**26125070 - Trần Nhật Tân (TV4)**

| Item         | Details                                                      |
| ------------ | ------------------------------------------------------------ |
| AI Tool      | ChatGPT                                                      |
| Provider     | OpenAI                                                       |
| Platform     | ChatGPT (web chat)                                           |
| Model        | GPT-6                                                        |
| Usage Period | 17/11/2026 – 20/11/2026                                      |
| Purposes     | Statistics implementation guidance, code explanation, test preparation, integration debugging, and Git/GitHub guidance. |
| Evidence     | ChatGPT conversation history and ummarized prompt logs provided in Section 3. |

---

## 3. Prompt Log

**25125049 - Trần Hồng Vy (TV1)**

- **AI Tool:** ChatGPT (OpenAI)
- **Purpose 1: Core Implementation and Integration.**
  - **Prompt 1:** "What is the best way to organize the menu navigation and input validation in FinanceManager?"
  - **Prompt 2:** "Review my current implementation. Are there any unnecessary operations or potential logic errors?"
  - **Prompt 3:** "Check my teammates' Pull Requests for interface compatibility, coding conventions, and integration issues."
- **Purpose 2: Testing and Debugging.**
  - **Prompt 4:** "What additional test cases should I include to cover invalid inputs and edge cases?"
  - **Prompt 5:** "Is there a potential integer overflow in the wallet balance operations? How should it be handled?"
  - **Prompt 6:** "Here are my regression test results. Is there anything else that needs to be verified before merging?"
- **Purpose 3: Git Management and Documentation.**
  - **Prompt 7:** "What would be an appropriate commit message for these changes?"
  - **Prompt 8:** "Update the weekly report and AI usage declaration to reflect the work completed during Sprint 2."

**26125041 - Lê Thiện Khôi (TV2)**

- **AI Tool:** Gemini (Google)
- **Purpose 1: Implement Data Models and Wallet Balance Logic in RAM.**
  - **Prompt 1:** “Based on the header files we defined in Week 1, help me implement the .cpp files for Wallet, IncomeSource, and ExpenseCategory.”
  - **Prompt 2:** “Write the logic for Wallet balance management (deposit, withdraw, canWithdraw, adjustBalance), ensuring we validate amounts > 0 and handle the allowNegative flag correctly.”
  - **Prompt 3:** “Ensure that all model operations check the isDeleted_ soft-deletion flag so we don't accidentally modify removed records.”
- **Purpose 2: Implement Manager Classes (CRUD operations and ID Management).**
  - **Prompt 4:** “Implement WalletManager.cpp with add, edit, delete, and view operations using the custom DynamicArray interface.”
  - **Prompt 5:** “Write the logic for nextId_ generation. Ensure IDs start from 1, auto-increment, and are strictly never reused even after a soft deletion.”
  - **Prompt 6:** “Implement the lookup methods (findIndexById, findIndexByName) so they explicitly ignore records where isDeleted() is true.”
  - **Prompt 7:** “Implement IncomeSourceManager and ExpenseCategoryManager following the exact same CRUD and soft-deletion logic as WalletManager.”
- **Purpose 3: Write Integration Tests and Verify Compilation.**
  - **Prompt 8:** “Create a standalone test driver test_master_data.cpp with assertions to verify Wallet and Category CRUD operations in RAM.”
  - **Prompt 9:** “Add edge-case tests to the driver: insufficient funds during withdrawal, duplicate name creation, and operations on soft-deleted items.”
  - **Prompt 10:** “Provide a mock DynamicArray.h and zip all the generated .cpp and .h files for Sprint 2 so I can easily compile and test them locally using MSYS2/MinGW before integrating with TV1.”

**26125089 - Dương Thị Nhã Uyên (TV3)**

- **AI Tool:** Claude (Anthropic)
- **Purpose 1: Understand the Week 2 task and get the latest code from GitHub.**
  - **Prompt 1:** “It is the second week of the project, and now I have to pull everything from GitHub.”
  - **Prompt 2:** “I can’t open Git Bash.” (Attached a screenshot of the right-click menu.)
  - **Prompt 3:** “This is my task this week.” (Attached the Transaction Models checklist, then pasted the full Week 2 task list: Transaction Models, Transaction Management, Validation & Wallet Balance, Testing & Integration, Acceptance Criteria.)
- **Purpose 2: Implement the transaction models and TransactionManager.**
  - **Prompt 4:** “The file Transaction.cpp should contain the functions, but you haven’t written anything for me yet.”
  - **Prompt 5:** “If you need to see any file, tell me.” (Then uploaded Transaction.h, Income.h, Expense.h, TransactionManager.h, Wallet.h, WalletManager.h, Date.h, DynamicArray.h and CODING_RULES.md, and asked: “Is this enough?”)
  - **Prompt 6:** “Here are the two missing files.” (Uploaded IncomeSourceManager.h and ExpenseCategoryManager.h.)
- **Purpose 3: Understand the code.**
  - **Prompt 7:** “Now explain the code to me, starting with the Transaction file.”
  - **Prompt 8:** “Explain the Income file.”
  - **Prompt 9:** “What does Transaction::Transaction mean?”
  - **Prompt 10:** “Why is there a ~ in Transaction::~Transaction()?”
  - **Prompt 11:** “I don’t understand this.” (Pasted the TransactionManager constructor and its initializer list.)
  - **Prompt 12:** Asked for an explanation of the Expense Category check (the nullptr check and exists() call) in addExpense.
  - **Prompt 13:** Asked for an explanation of the code that creates the Expense record, calls withdraw(), and rolls back on failure.
  - **Prompt 14:** Asked what `m_transactions.remove(m_transactions.size() - 1)` does.
  - **Prompt 15:** Asked for an explanation of getTransactionAt, getTransactionById, getTransactionCount and findTransactionIndex.
  - **Prompt 16:** “Next.” (Attached the Validation & Wallet Balance checklist.) Then: “So the second-to-last item is not done correctly yet, what should I do now?” and “No, the one above.”
- **Purpose 4: Test and verify the work.**
  - **Prompt 17:** “Can I test now? I want to see if it runs.”
  - **Prompt 18:** Attached a screenshot of the compile error “src/managers/*.cpp: Invalid argument”.
  - **Prompt 19:** “TV1 and TV2 have not written those two files yet.”
  - **Prompt 20:** “This is the Testing & Integration checklist. I can’t test yet, right? I have to wait for the other members.”
  - **Prompt 21:** “I ran it and no errors appeared.” (Result of the syntax-only compile check.)
- **Purpose 5: Troubleshoot Git and submit the work.**
  - **Prompt 22:** “I remember I already committed.”
  - **Prompt 23:** Attached a screenshot of the error from `git switch feat/transaction-models`: local changes would be overwritten.
  - **Prompt 24:** Attached screenshots of git status, git log and git diff to find out why the committed files were empty.
  - **Prompt 25:** Attached a screenshot of a successful git push of `feat/transaction-implementation`.
  - **Prompt 26:** “Create the AI usage declaration like this sample.” (Attached the Week 01 declaration.)

**26125070 - Trần Nhật Tân (TV4)**

- **AI Tool:** ChatGPT (OpenAI)

- **Purpose 1: Understand the Sprint 2 task and work through it in stages.**
  - **Prompt 1:** Help me follow the attached Sprint 2 task list one manageable step at a time, and wait for my reply before the next task.
  - **Prompt 2:** Explain which tasks have been completed and keep a progress note after each completed step.
- **Purpose 2: Implement and explain the statistics module.**
  - **Prompt 3:** Use the team headers to help implement calculateSummary() for Income, Expense and Net Balance over a selected date range.
  - **Prompt 4:** Explain how includes find headers in the include folder and how statistics access TransactionManager data.
- **Purpose 3: Prepare and verify statistics tests.**
  - **Prompt 5:** Help prepare tests for totals, date boundaries, invalid ranges, empty results and preservation of wallet balances.
  - **Prompt 6:** Explain what the test can check before TV2 and TV3 implementations are available, and what needs to change after integration.
- **Purpose 4: Manage commits and submit the work.**
  - **Prompt 7:** Guide me through updating my existing branch from main, staging files, creating commits and pushing the branch.
  - **Prompt 8:** Help me create a draft pull request and explain whether later commits update that same pull request.
- **Purpose 5: Investigate integration problems.**
  - **Prompt 9:** Help combine the TV2 and TV3 implementations on an integration branch and build the statistics tests.
  - **Prompt 10:** Explain the DynamicArray API errors after integration and help identify a compatible shared implementation.
  - **Prompt 11:** Help fix the Invalid date exception encountered while running tests and handle invalid date input in the menu.
  - **Prompt 12:** Explain stash and cherry-pick so I can preserve the core fix and move the test/menu fixes to my feature branch.
