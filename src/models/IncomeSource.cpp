#include "models/IncomeSource.h"

IncomeSource::IncomeSource() : id_(INVALID_ID), name_(""), isDeleted_(false) {}
IncomeSource::IncomeSource(int id, const std::string& name, bool isDeleted) : id_(id), name_(name), isDeleted_(isDeleted) {}

int IncomeSource::getId() const { return id_; }
const std::string& IncomeSource::getName() const { return name_; }
bool IncomeSource::isDeleted() const { return isDeleted_; }
bool IncomeSource::isValid() const { return (id_ > INVALID_ID) && (!isDeleted_) && (!name_.empty()); }

bool IncomeSource::setName(const std::string& name) {
    if (name.empty()) return false;
    name_ = name;
    return true;
}

void IncomeSource::setDeleted(bool deleted) { isDeleted_ = deleted; }

IncomeSourceRecord IncomeSource::toRecord(uint32_t nameOffset, uint32_t nameLength) const {
    IncomeSourceRecord record{};
    record.id = static_cast<int32_t>(id_);
    record.nameOffset = nameOffset;
    record.nameLength = nameLength;
    record.isDeleted = static_cast<uint8_t>(isDeleted_ ? 1 : 0);
    return record;
}

IncomeSource IncomeSource::fromRecord(const IncomeSourceRecord& record, const std::string& resolvedName) {
    return IncomeSource(static_cast<int>(record.id), resolvedName, record.isDeleted != 0);
}
