/**
 * @file IncomeSource.h
 * @author TV2 (Khoi)
 * @brief Defines the income source model and binary record.
 */

#ifndef MODELS_INCOME_SOURCE_H
#define MODELS_INCOME_SOURCE_H

#include <cstdint>
#include <string>
#include <type_traits>

// Fixed-size record for binary storage.
// Wallet names are stored separately in the Name Pool.
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
    // Constants
    static constexpr int INVALID_ID = 0;

    // Constructors
    IncomeSource();
    IncomeSource(int id, const std::string& name, bool isDeleted = false);

    // Getters
    int getId() const;
    const std::string& getName() const;
    bool isDeleted() const;
    bool isValid() const;

    // Setters
    void setName(const std::string& name);
    void setDeleted(bool deleted);

    // Binary Record Conversion
    IncomeSourceRecord toRecord(uint32_t nameOffset, uint32_t nameLength) const;
    static IncomeSource fromRecord(const IncomeSourceRecord& record, const std::string& resolvedName);

private:
    // Private Members
    int id_;
    std::string name_;
    bool isDeleted_;
};

#endif // MODELS_INCOME_SOURCE_H
