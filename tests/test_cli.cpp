/**
 * @file test_cli.cpp
 * @author TV1 (Vy)
 * @brief Tests integrated CLI workflows and input validation.
 */

#include <iostream>
#include <sstream>
#include <string>

#include "core/FinanceManager.h"

// Test Helpers
std::string runCli(const std::string& input) {
    std::istringstream inputStream(input);
    std::ostringstream outputStream;

    std::streambuf* oldInput = std::cin.rdbuf(inputStream.rdbuf());
    std::streambuf* oldOutput = std::cout.rdbuf(outputStream.rdbuf());

    FinanceManager manager;
    manager.run();

    std::cin.rdbuf(oldInput);
    std::cout.rdbuf(oldOutput);

    return outputStream.str();
}

bool expectContains(const std::string& output,
                    const std::string& expected,
                    const std::string& testName) {
    if (output.find(expected) == std::string::npos) {
        std::cout << "FAIL: " << testName << "\n";
        std::cout << "Expected: " << expected << "\n";
        return false;
    }

    std::cout << "PASS: " << testName << "\n";
    return true;
}

// Integration Tests
bool testCompleteWorkflow() {
    const std::string input =
        "4\n"
        "1\n"
        "Cash\n"
        "1000000\n"
        "0\n"
        "1\n"
        "5\n"
        "1\n"
        "Salary\n"
        "0\n"
        "1\n"
        "1\n"
        "1\n"
        "500000\n"
        "01/11/2026\n"
        "Monthly salary\n"
        "2\n"
        "0\n"
        "2\n"
        "5\n"
        "1\n"
        "Food\n"
        "0\n"
        "1\n"
        "1\n"
        "1\n"
        "200000\n"
        "20/11/2026\n"
        "Food expense\n"
        "2\n"
        "1\n"
        "1\n"
        "1\n"
        "2000000\n"
        "0\n"
        "3\n"
        "1\n"
        "01/11/2026\n"
        "30/11/2026\n"
        "1\n"
        "20/11/2026\n"
        "20/11/2026\n"
        "1\n"
        "01/12/2026\n"
        "31/12/2026\n"
        "1\n"
        "30/11/2026\n"
        "01/11/2026\n"
        "0\n"
        "4\n"
        "2\n"
        "0\n"
        "0\n";

    const std::string output = runCli(input);
    bool passed = true;

    passed &= expectContains(output, "Wallet added successfully.", "Create wallet");
    passed &= expectContains(output, "Income source added successfully.", "Create income source");
    passed &= expectContains(output, "Expense category added successfully.", "Create expense category");
    passed &= expectContains(output, "Income added successfully.", "Add income");
    passed &= expectContains(output, "Current wallet balance: 1500000 VND", "Balance after income");
    passed &= expectContains(output, "Expense added successfully.", "Add expense");
    passed &= expectContains(output, "Current wallet balance: 1300000 VND", "Balance after expense");

    passed &= expectContains(output, "===== INCOME LIST =====\nID: 1 | Income", "View income");
    passed &= expectContains(output, "===== EXPENSE LIST =====\nID: 2 | Expense", "View expense");

    passed &= expectContains(output, "Insufficient wallet balance.", "Reject excessive expense");

    passed &= expectContains(output,
        "Total Income: 500000 VND\n"
        "Total Expense: 200000 VND\n"
        "Net Balance: 300000 VND",
        "Monthly summary");

    passed &= expectContains(output,
        "Total Income: 0 VND\n"
        "Total Expense: 200000 VND\n"
        "Net Balance: -200000 VND",
        "Single-day summary");

    passed &= expectContains(output,
        "Total Income: 0 VND\n"
        "Total Expense: 0 VND\n"
        "Net Balance: 0 VND",
        "Empty date range");

    passed &= expectContains(output, "Unable to calculate summary.", "Reject reversed date range");

    passed &= expectContains(output, "ID: 1 | Name: Cash | Balance: 1300000", "Final wallet balance");

    passed &= expectContains(output, "Goodbye!", "Exit application");

    return passed;
}

bool testMissingMasterData() {
    const std::string input =
        "1\n"
        "1\n"
        "0\n"
        "2\n"
        "1\n"
        "0\n"
        "4\n"
        "1\n"
        "Cash\n"
        "1000000\n"
        "0\n"
        "1\n"
        "1\n"
        "0\n"
        "2\n"
        "1\n"
        "0\n"
        "0\n";

    const std::string output = runCli(input);
    bool passed = true;

    passed &= expectContains(output, "Please create a wallet first.", "Missing wallet");
    passed &= expectContains(output, "Please create an income source first.", "Missing income source");
    passed &= expectContains(output, "Please create an expense category first.", "Missing expense category");

    return passed;
}

bool testInvalidInput() {
    const std::string input =
        "abc\n"
        "999\n"
        "4\n"
        "1\n"
        "\n"
        "Cash\n"
        "abc\n"
        "1000000\n"
        "0\n"
        "1\n"
        "5\n"
        "1\n"
        "Salary\n"
        "0\n"
        "1\n"
        "999\n"
        "1\n"
        "1\n"
        "999\n"
        "1\n"
        "1\n"
        "1\n"
        "0\n"
        "1\n"
        "1\n"
        "1\n"
        "-50000\n"
        "1\n"
        "1\n"
        "1\n"
        "500000\n"
        "31/11/2026\n"
        "1 11 2026\n"
        "01/11/2026\n"
        "\n"
        "0\n"
        "0\n";

    const std::string output = runCli(input);
    bool passed = true;

    passed &= expectContains(output, "Invalid input. Please enter an integer.", "Reject non-integer input");
    passed &= expectContains(output, "Invalid option. Please try again.", "Reject invalid menu option");
    passed &= expectContains(output, "Input cannot be empty.", "Reject empty name");
    passed &= expectContains(output, "Invalid amount. Please enter an integer.", "Reject invalid money input");

    passed &= expectContains(output, "Wallet not found.", "Reject invalid wallet ID");
    passed &= expectContains(output, "Income source not found.", "Reject invalid income source ID");

    passed &= expectContains(output, "Amount must be greater than zero.", "Reject non-positive amount");
    passed &= expectContains(output, "Invalid date.", "Reject invalid date");
    passed &= expectContains(output, "Invalid format. Please use DD/MM/YYYY.", "Reject invalid date format");

    passed &= expectContains(output, "Income added successfully.", "Recover after invalid input");
    passed &= expectContains(output, "Current wallet balance: 1500000 VND", "Balance preserved after invalid input");

    return passed;
}

bool testEmptyTransactions() {
    const std::string input =
        "1\n"
        "2\n"
        "0\n"
        "2\n"
        "2\n"
        "0\n"
        "0\n";

    const std::string output = runCli(input);
    bool passed = true;

    passed &= expectContains(output,
        "===== INCOME LIST =====\n"
        "No transactions found.",
        "Empty income list");

    passed &= expectContains(output,
        "===== EXPENSE LIST =====\n"
        "No transactions found.",
        "Empty expense list");

    return passed;
}

bool testEndOfInput() {
    const std::string output = runCli("4\n1\n");

    return expectContains(output, "Wallet name: ", "Handle end of input");
}

bool testRecurringWorkflow() {
    const std::string input =
        "4\n1\nCash\n100000\n0\n"
        "1\n5\n1\nSalary\n0\n0\n"
        "2\n5\n1\nRent\n0\n0\n"
        "5\n"
        "1\n1\n1\n1\n50000\n01/11/2026\n5\nSalary\n0\n"
        "1\n2\n1\n1\n30000\n01/11/2026\n10\nRent\n0\n"
        "5\n30/11/2026\n"
        "3\n1\n1\n1\n70000\n01/11/2026\n15\nUpdated Salary\n0\n1\n"
        "5\n30/11/2026\n"
        "4\n2\n"
        "5\n31/12/2026\n"
        "2\n0\n"
        "1\n2\n0\n"
        "2\n2\n0\n"
        "4\n2\n0\n"
        "0\n";

    const std::string output = runCli(input);
    bool passed = true;

    passed &= expectContains(output,
        "Recurring transaction created successfully.",
        "Create recurring transaction");

    passed &= expectContains(output,
        "Generated transactions: 2\n"
        "Monthly transactions generated successfully.",
        "Generate recurring income and expense");

    passed &= expectContains(output,
        "Recurring transaction updated successfully.",
        "Edit generated schedule");

    passed &= expectContains(output,
        "Generated transactions: 0\n"
        "No recurring transactions are due.",
        "Prevent duplicate generation after editing");

    passed &= expectContains(output,
        "Recurring transaction deleted successfully.",
        "Delete generated schedule");

    passed &= expectContains(output,
        "Generated transactions: 1\n"
        "Monthly transactions generated successfully.",
        "Generate next monthly occurrence");

    passed &= expectContains(output,
        "ID: 1 | Income | Day 15 of each month | Wallet: 1 | "
        "70000 VND | Updated Salary | Start: 1/11/2026 | "
        "End: none | Last generated: 15/12/2026 | Active",
        "View updated recurring schedule");

    passed &= expectContains(output,
        "ID: 1 | Income | 5/11/2026 | Wallet: 1 | 50000 VND | Salary",
        "Preserve original generated income");

    passed &= expectContains(output,
        "ID: 2 | Expense | 10/11/2026 | Wallet: 1 | 30000 VND | Rent",
        "Preserve expense after deleting schedule");

    passed &= expectContains(output,
        "ID: 3 | Income | 15/12/2026 | Wallet: 1 | 70000 VND | Updated Salary",
        "Apply edited schedule to future occurrence");

    passed &= expectContains(output,
        "ID: 1 | Name: Cash | Balance: 190000",
        "Final balance after recurring workflow");

    return passed;
}

bool testRecurringPartialFailure() {
    const std::string input =
        "4\n1\nCash\n10000\n0\n"
        "1\n5\n1\nSalary\n0\n0\n"
        "2\n5\n1\nRent\n0\n0\n"
        "5\n"
        "1\n2\n1\n1\n30000\n01/11/2026\n5\nRent\n0\n"
        "1\n1\n1\n1\n50000\n01/11/2026\n10\nSalary\n0\n"
        "5\n30/11/2026\n"
        "5\n30/11/2026\n"
        "5\n30/11/2026\n"
        "0\n"
        "4\n2\n0\n"
        "0\n";

    const std::string output = runCli(input);
    bool passed = true;

    passed &= expectContains(output,
        "Generated transactions: 1\n"
        "Some recurring transactions could not be generated.",
        "Report partial generation failure");

    passed &= expectContains(output,
        "Generated transactions: 1\n"
        "Monthly transactions generated successfully.",
        "Retry failed recurring expense");

    passed &= expectContains(output,
        "Generated transactions: 0\n"
        "No recurring transactions are due.",
        "Prevent duplicates after retry");

    passed &= expectContains(output,
        "ID: 1 | Name: Cash | Balance: 30000",
        "Final balance after retry");

    return passed;
}

bool testRecurringInvalidInput() {
    const std::string input =
        "5\n1\n0\n"
        "4\n1\nCash\n100000\n0\n"
        "5\n1\n1\n0\n"
        "1\n5\n1\nSalary\n0\n0\n"
        "5\n"
        "1\n9\n"
        "1\n1\n999\n"
        "1\n1\n1\n999\n"
        "1\n1\n1\n1\n-1000\n"
        "1\n1\n1\n1\n50000\n01/11/2026\n32\n"
        "1\n1\n1\n1\n50000\n01/11/2026\n5\nSalary\n9\n"
        "1\n1\n1\n1\n50000\n01/11/2026\n5\nSalary\n1\n31/10/2026\n"
        "2\n"
        "0\n"
        "4\n2\n0\n"
        "0\n";

    const std::string output = runCli(input);
    bool passed = true;

    passed &= expectContains(output, "Please create a wallet first.", "Reject recurring creation without wallet");

    passed &= expectContains(output, "Please create an income source first.", "Reject recurring creation without source");

    passed &= expectContains(output, "Invalid transaction type.", "Reject invalid recurring type");

    passed &= expectContains(output, "Wallet not found.", "Reject invalid recurring wallet");

    passed &= expectContains(output, "Income source not found.", "Reject invalid recurring source");

    passed &= expectContains(output, "Amount must be greater than zero.", "Reject invalid recurring amount");

    passed &= expectContains(output, "Invalid day of month.", "Reject invalid monthly day");

    passed &= expectContains(output, "Invalid option.", "Reject invalid end date option");

    passed &= expectContains(output, "End date cannot be before start date.", "Reject reversed recurring dates");

    passed &= expectContains(output, "No recurring transactions.", "Invalid inputs create no schedules");

    passed &= expectContains(output, "ID: 1 | Name: Cash | Balance: 100000", "Balance preserved after invalid recurring input");

    return passed;
}

bool testRecurringEndOfInput() {
    const std::string input =
        "4\n1\nCash\n100000\n0\n"
        "1\n5\n1\nSalary\n0\n0\n"
        "5\n"
        "1\n1\n1\n1\n";

    const std::string output = runCli(input);

    return expectContains(output, "Amount (VND): ", "Handle end of input during recurring creation");
}

int main() {
    std::cout << "===== CLI INTEGRATION TESTS =====\n";

    bool passed = true;

    std::cout << "\n--- Complete Workflow ---\n";
    passed &= testCompleteWorkflow();

    std::cout << "\n--- Missing Master Data ---\n";
    passed &= testMissingMasterData();

    std::cout << "\n--- Input Validation ---\n";
    passed &= testInvalidInput();

    std::cout << "\n--- Empty Transactions ---\n";
    passed &= testEmptyTransactions();

    std::cout << "\n--- End of Input ---\n";
    passed &= testEndOfInput();

    std::cout << "\n--- Recurring Workflow ---\n";
    passed &= testRecurringWorkflow();

    std::cout << "\n--- Recurring Partial Failure ---\n";
    passed &= testRecurringPartialFailure();

    std::cout << "\n--- Recurring Input Validation ---\n";
    passed &= testRecurringInvalidInput();

    std::cout << "\n--- Recurring End of Input ---\n";
    passed &= testRecurringEndOfInput();

    if (!passed) {
        std::cout << "\nSome CLI integration tests failed.\n";
        return 1;
    }

    std::cout << "\nAll CLI integration tests passed!\n";

    return 0;
}
