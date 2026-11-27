/**
 * @file IncomeSource.cpp
 * @author TV2 (Khoi)
 * @brief Implements income source operations.
 */

#include "models/IncomeSource.h"

IncomeSource::IncomeSource() : m_id(INVALID_ID), m_name(""), m_isDeleted(false) {}

IncomeSource::IncomeSource(int id, const std::string& name, bool isDeleted) : m_id(id), m_name(name), m_isDeleted(isDeleted) {}

bool IncomeSource::isValid() const { return (m_id > INVALID_ID) && (!m_isDeleted) && (!m_name.empty()); }

int IncomeSource::getId() const { return m_id; }

const std::string& IncomeSource::getName() const { return m_name; }

bool IncomeSource::isDeleted() const { return m_isDeleted; }

bool IncomeSource::setName(const std::string& name) {
    if (name.empty()) return false;

    m_name = name;
    return true;
}

void IncomeSource::setDeleted(bool deleted) { m_isDeleted = deleted; }

IncomeSourceRecord IncomeSource::toRecord(uint32_t nameOffset, uint32_t nameLength) const {
    IncomeSourceRecord record{};
    record.id = static_cast<int32_t>(m_id);
    record.nameOffset = nameOffset;
    record.nameLength = nameLength;
    record.isDeleted = static_cast<uint8_t>(m_isDeleted ? 1 : 0);

    return record;
}

IncomeSource IncomeSource::fromRecord(const IncomeSourceRecord& record, const std::string& resolvedName) {
    return IncomeSource(static_cast<int>(record.id), resolvedName, record.isDeleted != 0);
}
