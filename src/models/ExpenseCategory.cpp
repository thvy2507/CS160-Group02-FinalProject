/**
 * @file ExpenseCategory.cpp
 * @author TV2 (Khoi)
 * @brief Implements expense category operations.
 */

#include "models/ExpenseCategory.h"

ExpenseCategory::ExpenseCategory() : m_id(INVALID_ID), m_name(""), m_isDeleted(false) {}

ExpenseCategory::ExpenseCategory(int id, const std::string& name, bool isDeleted) : m_id(id), m_name(name), m_isDeleted(isDeleted) {}

bool ExpenseCategory::isValid() const { return (m_id > INVALID_ID) && (!m_isDeleted) && (!m_name.empty()); }

int ExpenseCategory::getId() const { return m_id; }

const std::string& ExpenseCategory::getName() const { return m_name; }

bool ExpenseCategory::isDeleted() const { return m_isDeleted; }

bool ExpenseCategory::setName(const std::string& name) {
    if (name.empty()) return false;

    m_name = name;
    return true;
}

void ExpenseCategory::setDeleted(bool deleted) { m_isDeleted = deleted; }

ExpenseCategoryRecord ExpenseCategory::toRecord(uint32_t nameOffset, uint32_t nameLength) const {
    ExpenseCategoryRecord record{};
    record.id = static_cast<int32_t>(m_id);
    record.nameOffset = nameOffset;
    record.nameLength = nameLength;
    record.isDeleted = static_cast<uint8_t>(m_isDeleted ? 1 : 0);

    return record;
}

ExpenseCategory ExpenseCategory::fromRecord(const ExpenseCategoryRecord& record, const std::string& resolvedName) {
    return ExpenseCategory(static_cast<int>(record.id), resolvedName, record.isDeleted != 0);
}
