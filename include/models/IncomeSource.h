#ifndef MODELS_INCOME_SOURCE_H
#define MODELS_INCOME_SOURCE_H

#include <cstdint>
#include <string>
#include <type_traits>

struct IncomeSourceRecord {
    int32_t id;
    uint32_t nameOffset;
    uint32_t nameLength;
    uint8_t isDeleted;
};

static_assert(std::is_trivially_copyable_v<IncomeSourceRecord>,
              "IncomeSourceRecord must be trivially copyable for binary file I/O.");

class IncomeSource {
public:
    static constexpr int INVALID_ID = 0;

    IncomeSource();
    IncomeSource(int id, const std::string& name, bool isDeleted = false);

    int getId() const;
    const std::string& getName() const;
    bool isDeleted() const;
    bool isValid() const;

    bool setName(const std::string& name);
    void setDeleted(bool deleted);

    IncomeSourceRecord toRecord(uint32_t nameOffset, uint32_t nameLength) const;
    static IncomeSource fromRecord(const IncomeSourceRecord& record, const std::string& resolvedName);

private:
    int id_;
    std::string name_;
    bool isDeleted_;
};

#endif // MODELS_INCOME_SOURCE_H
