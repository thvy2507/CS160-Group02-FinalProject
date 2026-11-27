/**
 * @file ExpenseCategory.h
 * @author TV2 (Khoi)
 * @brief Declares the expense category model and binary record structure.
 */

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
    // Constants
    static constexpr int INVALID_ID = 0;

    // Constructors
    ExpenseCategory();
    ExpenseCategory(int id, const std::string& name, bool isDeleted = false);

    // Validation
    bool isValid() const;

    // Getters
    int getId() const;
    const std::string& getName() const;
    bool isDeleted() const;

    // Setters
    bool setName(const std::string& name);
    void setDeleted(bool deleted);

    // Binary Record Conversion
    ExpenseCategoryRecord toRecord(uint32_t nameOffset, uint32_t nameLength) const;
    static ExpenseCategory fromRecord(const ExpenseCategoryRecord& record, const std::string& resolvedName);

private:
    // Private Members
    int m_id;
    std::string m_name;
    bool m_isDeleted;
};

#endif // MODELS_EXPENSE_CATEGORY_H
