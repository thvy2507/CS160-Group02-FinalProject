#ifndef MODELS_EXPENSE_CATEGORY_H
#define MODELS_EXPENSE_CATEGORY_H

#include <cstdint>
#include <string>
#include <type_traits>

// Ban ghi kich thuoc co dinh (Fixed-size Record) cho ExpenseCategory
struct ExpenseCategoryRecord {
    int32_t id;             // Object ID duy nhat
    uint32_t nameOffset;    // Vi tri byte bat dau cua ten danh muc chi trong Name Pool
    uint32_t nameLength;    // Do dai ten danh muc chi (tinh bang byte)
    uint8_t isDeleted;      // Co xoa mem: 0 = Active, 1 = Deleted
};

static_assert(std::is_trivially_copyable_v<ExpenseCategoryRecord>,
              "ExpenseCategoryRecord must be trivially copyable for binary file I/O.");

class ExpenseCategory {
public:
    static constexpr int INVALID_ID = 0;

    // Default constructor (bat buoc cho DynamicArray<ExpenseCategory>)
    ExpenseCategory();

    // Constructor co tham so
    ExpenseCategory(int id, const std::string& name, bool isDeleted = false);

    // Getters
    int getId() const;
    const std::string& getName() const;
    bool isDeleted() const;
    bool isValid() const;

    // Setters
    void setName(const std::string& name);
    void setDeleted(bool deleted);

    // Chuyen doi phuc vu Binary Storage
    ExpenseCategoryRecord toRecord(uint32_t nameOffset, uint32_t nameLength) const;
    static ExpenseCategory fromRecord(const ExpenseCategoryRecord& record, const std::string& resolvedName);

private:
    int id_;
    std::string name_;
    bool isDeleted_;
};

#endif // MODELS_EXPENSE_CATEGORY_H