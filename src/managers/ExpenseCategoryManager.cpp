/**
 * @file ExpenseCategoryManager.cpp
 * @author TV2 (Khoi)
 * @brief Implements expense category management operations.
 */

#include "managers/ExpenseCategoryManager.h"

#include <fstream>
#include <iostream>
#include <limits>

#include "core/BinaryIO.h"

namespace {

    bool writeExpenseCategoryRecord(std::ostream& file, const ExpenseCategoryRecord& record) {
        return BinaryIO::writeInt32(file, record.id)
            && BinaryIO::writeUInt32(file, record.nameOffset)
            && BinaryIO::writeUInt32(file, record.nameLength)
            && BinaryIO::writeUInt8(file, record.isDeleted);
    }

    bool readExpenseCategoryRecord(std::istream& file, ExpenseCategoryRecord& record) {
        return BinaryIO::readInt32(file, record.id)
            && BinaryIO::readUInt32(file, record.nameOffset)
            && BinaryIO::readUInt32(file, record.nameLength)
            && BinaryIO::readUInt8(file, record.isDeleted);
    }

    bool isValidExpenseCategoryRecord(const ExpenseCategoryRecord& record, const std::string& name, const DynamicArray<ExpenseCategory>& categories) {

        if (record.id <= ExpenseCategory::INVALID_ID ||
            record.id == std::numeric_limits<int32_t>::max() ||
            record.isDeleted > 1 ||
            name.empty()) {
            return false;
        }

        for (int i = 0; i < categories.size(); ++i) {
            const ExpenseCategory& category = categories[i];
            if (category.getId() == record.id) {
                return false;
            }

            if (record.isDeleted == 0 &&
                !category.isDeleted() &&
                category.getName() == name) {
                return false;
            }
        }

        return true;
    }

} // namespace

ExpenseCategoryManager::ExpenseCategoryManager() : m_categories(), m_nextId(1) {}

int ExpenseCategoryManager::addExpenseCategory(const std::string& name) {
    if (m_nextId == std::numeric_limits<int32_t>::max() || name.empty() || findIndexByName(name) != -1) {
        return ExpenseCategory::INVALID_ID;
    }

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
    if (recordFilePath == namePoolPath) {
        return false;
    }

    std::ifstream recordFile(recordFilePath, std::ios::binary);
    std::ifstream namePool(namePoolPath, std::ios::binary);
    if (!recordFile || !namePool) {
        return false;
    }

    uint64_t recordFileSize, poolSize;
    BinaryIO::FileHeader header{};
    if (!BinaryIO::getFileSize(recordFile, recordFileSize) ||
        !BinaryIO::getFileSize(namePool, poolSize) ||
        !BinaryIO::readHeader(recordFile, header) ||
        !BinaryIO::validateHeader(
            header, BinaryIO::EXPENSE_CATEGORY_MAGIC,
            BinaryIO::EXPENSE_CATEGORY_RECORD_SIZE, recordFileSize,
            std::numeric_limits<int>::max() - 1) ||
        poolSize > std::numeric_limits<uint32_t>::max()) {
        return false;
    }

    DynamicArray<ExpenseCategory> tempCategories(header.recordCount == 0 ?
                                                 4 : static_cast<int>(header.recordCount)
                                                 );

    int maxId = 0;
    for (uint32_t i = 0; i < header.recordCount; ++i) {
        ExpenseCategoryRecord record{};
        std::string name;

        if (!readExpenseCategoryRecord(recordFile, record) ||
            !BinaryIO::readString(namePool, record.nameOffset, record.nameLength, poolSize, name) ||
            !isValidExpenseCategoryRecord(record, name, tempCategories)) {
            return false;
        }

        tempCategories.add(ExpenseCategory::fromRecord(record, name));
        if (record.id > maxId) {
            maxId = record.id;
        }
    }

    m_categories = tempCategories;
    m_nextId = maxId + 1;

    return true;
}

bool ExpenseCategoryManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    if (recordFilePath == namePoolPath) {
        return false;
    }

    std::ofstream recordFile(recordFilePath, std::ios::binary | std::ios::trunc);
    std::ofstream namePool(namePoolPath, std::ios::binary | std::ios::trunc);

    if (!recordFile || !namePool) {
        return false;
    }

    uint32_t recordCount = static_cast<uint32_t>(m_categories.size());
    if (!BinaryIO::writeHeader(recordFile, BinaryIO::EXPENSE_CATEGORY_MAGIC, BinaryIO::EXPENSE_CATEGORY_RECORD_SIZE, recordCount)) {
        return false;
    }

    for (int i = 0; i < m_categories.size(); ++i) {
        const ExpenseCategory& category = m_categories[i];

        uint32_t offset, length;
        if (!BinaryIO::appendString(namePool, category.getName(), offset, length)) {
            return false;
        }

        ExpenseCategoryRecord record = category.toRecord(offset, length);

        if (!writeExpenseCategoryRecord(recordFile, record)) {
            return false;
        }
    }

    recordFile.close();
    namePool.close();

    return static_cast<bool>(recordFile) && static_cast<bool>(namePool);
}
