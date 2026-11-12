#ifndef MANAGERS_EXPENSE_CATEGORY_MANAGER_H
#define MANAGERS_EXPENSE_CATEGORY_MANAGER_H

#include <string>
#include "core/DynamicArray.h"
#include "models/ExpenseCategory.h"

class ExpenseCategoryManager {
public:
    ExpenseCategoryManager();
    ~ExpenseCategoryManager() = default;

    // --- Thao tac CRUD chinh ---
    int addExpenseCategory(const std::string& name);
    bool editExpenseCategory(int categoryId, const std::string& newName);
    bool deleteExpenseCategory(int categoryId);
    void viewAllExpenseCategories() const;

    // --- Truy van & Tim kiem (Phuc vu TV3 Transaction & TV4 Statistics) ---
    bool exists(int categoryId) const;
    int findIndexById(int categoryId) const;
    int findIndexByName(const std::string& name) const;

    ExpenseCategory* getExpenseCategoryById(int categoryId);
    const ExpenseCategory* getExpenseCategoryById(int categoryId) const;
    std::string getExpenseCategoryName(int categoryId) const;

    const DynamicArray<ExpenseCategory>& getAllExpenseCategories() const;
    int getActiveCount() const;

    // --- Binary Persistence Hooks ---
    bool loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath);
    bool saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const;

private:
    DynamicArray<ExpenseCategory> categories_;
    int nextId_;
};

#endif // MANAGERS_EXPENSE_CATEGORY_MANAGER_H