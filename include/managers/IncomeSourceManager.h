/**
 * @file IncomeSourceManager.h
 * @author TV2 (Khoi)
 * @brief Manages income sources.
 */

#ifndef MANAGERS_INCOME_SOURCE_MANAGER_H
#define MANAGERS_INCOME_SOURCE_MANAGER_H

#include <string>
#include "core/DynamicArray.h"
#include "models/IncomeSource.h"

class IncomeSourceManager {
public:
    // Constructors & Destructor
    IncomeSourceManager();
    ~IncomeSourceManager() = default;

    // CRUD Operations
    int addIncomeSource(const std::string& name);
    bool editIncomeSource(int sourceId, const std::string& newName);
    bool deleteIncomeSource(int sourceId);
    void viewAllIncomeSources() const;

    // Search & Access
    bool exists(int sourceId) const;
    int findIndexById(int sourceId) const;
    int findIndexByName(const std::string& name) const;

    IncomeSource* getIncomeSourceById(int sourceId);
    const IncomeSource* getIncomeSourceById(int sourceId) const;
    std::string getIncomeSourceName(int sourceId) const;

    const DynamicArray<IncomeSource>& getAllIncomeSources() const;
    int getActiveCount() const;

    // Binary Persistence
    bool loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath);
    bool saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const;

private:
    // Private Members
    DynamicArray<IncomeSource> m_sources;
    int m_nextId;
};

#endif // MANAGERS_INCOME_SOURCE_MANAGER_H
