/**
 * @file ExpenseCategoryManager.cpp
 * @author TV2 (Khoi)
 * @brief Implements expense category management operations.
 */

#include "managers/ExpenseCategoryManager.h"

#include <iostream>

ExpenseCategoryManager::ExpenseCategoryManager() : m_categories(), m_nextId(1) {}

int ExpenseCategoryManager::addExpenseCategory(const std::string& name) {
    if (name.empty() || findIndexByName(name) != -1) return ExpenseCategory::INVALID_ID;

    int assignedId = m_nextId++;
    ExpenseCategory newCat(assignedId, name);
    m_categories.add(newCat);

    return assignedId;
}

void ExpenseCategoryManager::viewAllExpenseCategories() const {
    std::cout << "\n===== EXPENSE CATEGORY LIST =====\n";

    int activeCount = 0;
    for (int i = 0; i < m_categories.getSize(); ++i) {
        if (!m_categories[i].isDeleted()) {
            ++activeCount;
            std::cout << "ID: " << m_categories[i].getId() << " | Name: " << m_categories[i].getName() << "\n";
        }
    }

    if (activeCount == 0) std::cout << "No active expense categories found.\n";
}

bool ExpenseCategoryManager::editExpenseCategory(int categoryId, const std::string& newName) {
    if (newName.empty()) return false;

    int targetIdx = findIndexById(categoryId);
    if (targetIdx == -1) return false;

    int duplicateIdx = findIndexByName(newName);
    if (duplicateIdx != -1 && duplicateIdx != targetIdx) return false;

    return m_categories[targetIdx].setName(newName);
}

bool ExpenseCategoryManager::deleteExpenseCategory(int categoryId) {
    int targetIdx = findIndexById(categoryId);
    if (targetIdx == -1) return false;

    m_categories[targetIdx].setDeleted(true);
    return true;
}

bool ExpenseCategoryManager::exists(int categoryId) const { return findIndexById(categoryId) != -1; }

int ExpenseCategoryManager::findIndexById(int categoryId) const {
    if (categoryId <= ExpenseCategory::INVALID_ID) return -1;

    for (int i = 0; i < m_categories.getSize(); ++i) {
        if (!m_categories[i].isDeleted() && m_categories[i].getId() == categoryId) return i;
    }

    return -1;
}

int ExpenseCategoryManager::findIndexByName(const std::string& name) const {
    if (name.empty()) return -1;

    for (int i = 0; i < m_categories.getSize(); ++i) {
        if (!m_categories[i].isDeleted() && m_categories[i].getName() == name) return i;
    }

    return -1;
}

ExpenseCategory* ExpenseCategoryManager::getExpenseCategoryById(int categoryId) {
    int idx = findIndexById(categoryId);
    return (idx != -1) ? &m_categories[idx] : nullptr;
}

const ExpenseCategory* ExpenseCategoryManager::getExpenseCategoryById(int categoryId) const {
    int idx = findIndexById(categoryId);
    return (idx != -1) ? &m_categories[idx] : nullptr;
}

std::string ExpenseCategoryManager::getExpenseCategoryName(int categoryId) const {
    const ExpenseCategory* cat = getExpenseCategoryById(categoryId);
    return (cat != nullptr) ? cat->getName() : "";
}

const DynamicArray<ExpenseCategory>& ExpenseCategoryManager::getAllExpenseCategories() const { return m_categories; }

int ExpenseCategoryManager::getActiveCount() const {
    int count = 0;
    for (int i = 0; i < m_categories.getSize(); ++i) {
        if (!m_categories[i].isDeleted()) ++count;
    }

    return count;
}

bool ExpenseCategoryManager::loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath) {
    (void)recordFilePath; (void)namePoolPath; return false;
}

bool ExpenseCategoryManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    (void)recordFilePath; (void)namePoolPath; return false;
}
