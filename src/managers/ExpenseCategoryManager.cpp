#include "managers/ExpenseCategoryManager.h"
#include "core/BinaryIO.h"
#include <iostream>
#include <vector>

ExpenseCategoryManager::ExpenseCategoryManager() : categories_(), nextId_(1) {}

int ExpenseCategoryManager::addExpenseCategory(const std::string& name) {
    if (name.empty() || findIndexByName(name) != -1) return ExpenseCategory::INVALID_ID;
    int assignedId = nextId_++;
    categories_.pushBack(ExpenseCategory(assignedId, name));
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
    for (int i = 0; i < categories_.getSize(); ++i) {
        if (!categories_[i].isDeleted()) std::cout << "ID: " << categories_[i].getId() << " | Name: " << categories_[i].getName() << "\n";
    }
}
bool ExpenseCategoryManager::exists(int categoryId) const { return findIndexById(categoryId) != -1; }
int ExpenseCategoryManager::findIndexById(int categoryId) const {
    for (int i = 0; i < categories_.getSize(); ++i) {
        if (!categories_[i].isDeleted() && categories_[i].getId() == categoryId) return i;
    }
    return -1;
}
int ExpenseCategoryManager::findIndexByName(const std::string& name) const {
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
    return cat ? cat->getName() : "";
}
const DynamicArray<ExpenseCategory>& ExpenseCategoryManager::getAllExpenseCategories() const { return categories_; }
int ExpenseCategoryManager::getActiveCount() const {
    int c = 0;
    for (int i = 0; i < categories_.getSize(); ++i) if (!categories_[i].isDeleted()) c++;
    return c;
}

bool ExpenseCategoryManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    std::string namePool = "";
    std::vector<ExpenseCategoryRecord> records;
    for (int i = 0; i < categories_.getSize(); ++i) {
        const ExpenseCategory& cat = categories_[i];
        uint32_t offset = static_cast<uint32_t>(namePool.size());
        uint32_t len = static_cast<uint32_t>(cat.getName().size());
        namePool += cat.getName();
        records.push_back(cat.toRecord(offset, len));
    }
    if (!records.empty()) {
        if (!BinaryIO::writeBuffer(recordFilePath, records.data(), records.size() * sizeof(ExpenseCategoryRecord))) return false;
    } else { BinaryIO::writeBuffer(recordFilePath, "", 0); }
    if (!namePool.empty()) {
        if (!BinaryIO::writeBuffer(namePoolPath, namePool.data(), namePool.size())) return false;
    } else { BinaryIO::writeBuffer(namePoolPath, "", 0); }
    return true;
}

bool ExpenseCategoryManager::loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath) {
    if (!BinaryIO::fileExists(recordFilePath) || !BinaryIO::fileExists(namePoolPath)) return true;
    size_t recordSize = BinaryIO::getFileSize(recordFilePath);
    size_t poolSize = BinaryIO::getFileSize(namePoolPath);
    if (recordSize % sizeof(ExpenseCategoryRecord) != 0) return false;
    
    std::vector<ExpenseCategoryRecord> records(recordSize / sizeof(ExpenseCategoryRecord));
    std::string pool(poolSize, '\0');
    
    if (recordSize > 0 && !BinaryIO::readBuffer(recordFilePath, records.data(), recordSize)) return false;
    if (poolSize > 0 && !BinaryIO::readBuffer(namePoolPath, pool.data(), poolSize)) return false;

    DynamicArray<ExpenseCategory> tempCats;
    int maxId = 0;
    for (const auto& rec : records) {
        if (rec.nameOffset + rec.nameLength > poolSize) return false;
        std::string name = pool.substr(rec.nameOffset, rec.nameLength);
        tempCats.pushBack(ExpenseCategory::fromRecord(rec, name));
        if (rec.id > maxId) maxId = rec.id;
    }
    categories_ = tempCats;
    nextId_ = maxId + 1;
    return true;
}
