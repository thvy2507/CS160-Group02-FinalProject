#include "managers/ExpenseCategoryManager.h"
#include <iostream>

ExpenseCategoryManager::ExpenseCategoryManager() : categories_(), nextId_(1) {}

int ExpenseCategoryManager::addExpenseCategory(const std::string& name) {
    if (name.empty() || findIndexByName(name) != -1) return ExpenseCategory::INVALID_ID;
    int assignedId = nextId_++;
    ExpenseCategory newCat(assignedId, name);
    categories_.add(newCat);
    return assignedId;
}

bool ExpenseCategoryManager::editExpenseCategory(int categoryId, const std::string& newName) {
    if (newName.empty()) return false;
    int targetIdx = findIndexById(categoryId);
    if (targetIdx == -1) return false;
    int duplicateIdx = findIndexByName(newName);
    if (duplicateIdx != -1 && duplicateIdx != targetIdx) return false;
    return categories_[targetIdx].setName(newName);
}

bool ExpenseCategoryManager::deleteExpenseCategory(int categoryId) {
    int targetIdx = findIndexById(categoryId);
    if (targetIdx == -1) return false;
    categories_[targetIdx].setDeleted(true);
    return true;
}

void ExpenseCategoryManager::viewAllExpenseCategories() const {
    std::cout << "\n=== DANH SACH DANH MUC CHI TIEU ===\n";
    int activeCount = 0;
    for (int i = 0; i < categories_.getSize(); ++i) {
        if (!categories_[i].isDeleted()) {
            ++activeCount;
            std::cout << "ID: " << categories_[i].getId() << " | Ten danh muc: " << categories_[i].getName() << "\n";
        }
    }
    if (activeCount == 0) std::cout << "(Chua co danh muc nao dang hoat dong)\n";
}

bool ExpenseCategoryManager::exists(int categoryId) const { return findIndexById(categoryId) != -1; }

int ExpenseCategoryManager::findIndexById(int categoryId) const {
    if (categoryId <= ExpenseCategory::INVALID_ID) return -1;
    for (int i = 0; i < categories_.getSize(); ++i) {
        if (!categories_[i].isDeleted() && categories_[i].getId() == categoryId) return i;
    }
    return -1;
}

int ExpenseCategoryManager::findIndexByName(const std::string& name) const {
    if (name.empty()) return -1;
    for (int i = 0; i < categories_.getSize(); ++i) {
        if (!categories_[i].isDeleted() && categories_[i].getName() == name) return i;
    }
    return -1;
}

ExpenseCategory* ExpenseCategoryManager::getExpenseCategoryById(int categoryId) {
    int idx = findIndexById(categoryId);
    return (idx != -1) ? &categories_[idx] : nullptr;
}

const ExpenseCategory* ExpenseCategoryManager::getExpenseCategoryById(int categoryId) const {
    int idx = findIndexById(categoryId);
    return (idx != -1) ? &categories_[idx] : nullptr;
}

std::string ExpenseCategoryManager::getExpenseCategoryName(int categoryId) const {
    const ExpenseCategory* cat = getExpenseCategoryById(categoryId);
    return (cat != nullptr) ? cat->getName() : "";
}

const DynamicArray<ExpenseCategory>& ExpenseCategoryManager::getAllExpenseCategories() const { return categories_; }

int ExpenseCategoryManager::getActiveCount() const {
    int count = 0;
    for (int i = 0; i < categories_.getSize(); ++i) {
        if (!categories_[i].isDeleted()) ++count;
    }
    return count;
}

bool ExpenseCategoryManager::loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath) {
    (void)recordFilePath; (void)namePoolPath; return false;
}
bool ExpenseCategoryManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    (void)recordFilePath; (void)namePoolPath; return false;
}
