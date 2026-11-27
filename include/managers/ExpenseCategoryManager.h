/**
 * @file ExpenseCategoryManager.h
 * @author TV2 (Khoi)
 * @brief Declares expense category management operations.
 */

#ifndef MANAGERS_EXPENSE_CATEGORY_MANAGER_H
#define MANAGERS_EXPENSE_CATEGORY_MANAGER_H

#include <string>

#include "core/DynamicArray.h"
#include "models/ExpenseCategory.h"

class ExpenseCategoryManager {
public:
    // Constructors & Destructor
    ExpenseCategoryManager();
    ~ExpenseCategoryManager() = default;

    // CRUD Operations
    int addExpenseCategory(const std::string& name);
    void viewAllExpenseCategories() const;
    bool editExpenseCategory(int categoryId, const std::string& newName);
    bool deleteExpenseCategory(int categoryId);

    // Search & Access
    bool exists(int categoryId) const;
    int findIndexById(int categoryId) const;
    int findIndexByName(const std::string& name) const;

    ExpenseCategory* getExpenseCategoryById(int categoryId);
    const ExpenseCategory* getExpenseCategoryById(int categoryId) const;
    std::string getExpenseCategoryName(int categoryId) const;

    const DynamicArray<ExpenseCategory>& getAllExpenseCategories() const;
    int getActiveCount() const;

    // Binary File Operations
    bool loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath);
    bool saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const;

private:
    // Private Members
    DynamicArray<ExpenseCategory> m_categories;
    int m_nextId;
};

#endif // MANAGERS_EXPENSE_CATEGORY_MANAGER_H
