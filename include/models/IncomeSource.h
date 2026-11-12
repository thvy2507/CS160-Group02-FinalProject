#ifndef MODELS_INCOME_SOURCE_H
#define MODELS_INCOME_SOURCE_H

#include <cstdint>
#include <string>
#include <type_traits>

// Ban ghi kich thuoc co dinh (Fixed-size Record) cho IncomeSource
struct IncomeSourceRecord {
    int32_t id;             // Object ID duy nhat
    uint32_t nameOffset;    // Vi tri byte bat dau cua ten nguon thu trong Name Pool
    uint32_t nameLength;    // Do dai ten nguon thu (tinh bang byte)
    uint8_t isDeleted;      // Co xoa mem: 0 = Active, 1 = Deleted
};

static_assert(std::is_trivially_copyable_v<IncomeSourceRecord>,
              "IncomeSourceRecord must be trivially copyable for binary file I/O.");

class IncomeSource {
public:
    static constexpr int INVALID_ID = 0;

    // Default constructor (bat buoc cho DynamicArray<IncomeSource>)
    IncomeSource();

    // Constructor co tham so
    IncomeSource(int id, const std::string& name, bool isDeleted = false);

    // Getters
    int getId() const;
    const std::string& getName() const;
    bool isDeleted() const;
    bool isValid() const;

    // Setters
    void setName(const std::string& name);
    void setDeleted(bool deleted);

    // Chuyen doi phuc vu Binary Storage
    IncomeSourceRecord toRecord(uint32_t nameOffset, uint32_t nameLength) const;
    static IncomeSource fromRecord(const IncomeSourceRecord& record, const std::string& resolvedName);

private:
    int id_;
    std::string name_;
    bool isDeleted_;
};

#endif // MODELS_INCOME_SOURCE_H