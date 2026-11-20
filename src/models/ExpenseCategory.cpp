#include "models/ExpenseCategory.h"

ExpenseCategory::ExpenseCategory() : id_(INVALID_ID), name_(""), isDeleted_(false) {}
ExpenseCategory::ExpenseCategory(int id, const std::string& name, bool isDeleted) : id_(id), name_(name), isDeleted_(isDeleted) {}

int ExpenseCategory::getId() const { return id_; }
const std::string& ExpenseCategory::getName() const { return name_; }
bool ExpenseCategory::isDeleted() const { return isDeleted_; }
bool ExpenseCategory::isValid() const { return (id_ > INVALID_ID) && (!isDeleted_) && (!name_.empty()); }

bool ExpenseCategory::setName(const std::string& name) {
    if (name.empty()) return false;
    name_ = name;
    return true;
}

void ExpenseCategory::setDeleted(bool deleted) { isDeleted_ = deleted; }

ExpenseCategoryRecord ExpenseCategory::toRecord(uint32_t nameOffset, uint32_t nameLength) const {
    ExpenseCategoryRecord record{};
    record.id = static_cast<int32_t>(id_);
    record.nameOffset = nameOffset;
    record.nameLength = nameLength;
    record.isDeleted = static_cast<uint8_t>(isDeleted_ ? 1 : 0);
    return record;
}

ExpenseCategory ExpenseCategory::fromRecord(const ExpenseCategoryRecord& record, const std::string& resolvedName) {
    return ExpenseCategory(static_cast<int>(record.id), resolvedName, record.isDeleted != 0);
}
