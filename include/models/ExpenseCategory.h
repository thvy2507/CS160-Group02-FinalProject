#ifndef MODELS_EXPENSE_CATEGORY_H
#define MODELS_EXPENSE_CATEGORY_H

#include <cstdint>
#include <string>
#include <type_traits>

struct ExpenseCategoryRecord {
    int32_t id;
    uint32_t nameOffset;
    uint32_t nameLength;
    uint8_t isDeleted;
};

static_assert(std::is_trivially_copyable_v<ExpenseCategoryRecord>,
              "ExpenseCategoryRecord must be trivially copyable for binary file I/O.");

class ExpenseCategory {
public:
    static constexpr int INVALID_ID = 0;

    ExpenseCategory();
    ExpenseCategory(int id, const std::string& name, bool isDeleted = false);

    int getId() const;
    const std::string& getName() const;
    bool isDeleted() const;
    bool isValid() const;

    bool setName(const std::string& name);
    void setDeleted(bool deleted);

    ExpenseCategoryRecord toRecord(uint32_t nameOffset, uint32_t nameLength) const;
    static ExpenseCategory fromRecord(const ExpenseCategoryRecord& record, const std::string& resolvedName);

private:
    int id_;
    std::string name_;
    bool isDeleted_;
};

#endif // MODELS_EXPENSE_CATEGORY_H
