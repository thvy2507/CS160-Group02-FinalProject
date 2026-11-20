/**
 * @file IncomeSourceManager.cpp
 * @author TV2 (Khoi)
 * @brief Implements income source management operations.
 */

#include "managers/IncomeSourceManager.h"

#include <iostream>

IncomeSourceManager::IncomeSourceManager() : m_sources(), m_nextId(1) {}

int IncomeSourceManager::addIncomeSource(const std::string& name) {
    if (name.empty() || findIndexByName(name) != -1) return IncomeSource::INVALID_ID;

    int assignedId = m_nextId++;
    IncomeSource newSource(assignedId, name);
    m_sources.add(newSource);

    return assignedId;
}

void IncomeSourceManager::viewAllIncomeSources() const {
    std::cout << "\n===== INCOME SOURCE LIST =====\n";

    int activeCount = 0;
    for (int i = 0; i < m_sources.getSize(); ++i) {
        if (!m_sources[i].isDeleted()) {
            ++activeCount;

            std::cout << "ID: " << m_sources[i].getId() << " | Name: " << m_sources[i].getName() << "\n";
        }
    }
    if (activeCount == 0) std::cout << "No active income sources found.\n";
}

bool IncomeSourceManager::editIncomeSource(int sourceId, const std::string& newName) {
    if (newName.empty()) return false;

    int targetIdx = findIndexById(sourceId);
    if (targetIdx == -1) return false;

    int duplicateIdx = findIndexByName(newName);
    if (duplicateIdx != -1 && duplicateIdx != targetIdx) return false;

    return m_sources[targetIdx].setName(newName);
}

bool IncomeSourceManager::deleteIncomeSource(int sourceId) {
    int targetIdx = findIndexById(sourceId);
    if (targetIdx == -1) return false;

    m_sources[targetIdx].setDeleted(true);
    return true;
}

bool IncomeSourceManager::exists(int sourceId) const { return findIndexById(sourceId) != -1; }

int IncomeSourceManager::findIndexById(int sourceId) const {
    if (sourceId <= IncomeSource::INVALID_ID) return -1;
    for (int i = 0; i < m_sources.getSize(); ++i) {
        if (!m_sources[i].isDeleted() && m_sources[i].getId() == sourceId) return i;
    }

    return -1;
}

int IncomeSourceManager::findIndexByName(const std::string& name) const {
    if (name.empty()) return -1;
    for (int i = 0; i < m_sources.getSize(); ++i) {
        if (!m_sources[i].isDeleted() && m_sources[i].getName() == name) return i;
    }

    return -1;
}

IncomeSource* IncomeSourceManager::getIncomeSourceById(int sourceId) {
    int idx = findIndexById(sourceId);
    return (idx != -1) ? &m_sources[idx] : nullptr;
}

const IncomeSource* IncomeSourceManager::getIncomeSourceById(int sourceId) const {
    int idx = findIndexById(sourceId);
    return (idx != -1) ? &m_sources[idx] : nullptr;
}

std::string IncomeSourceManager::getIncomeSourceName(int sourceId) const {
    const IncomeSource* source = getIncomeSourceById(sourceId);
    return (source != nullptr) ? source->getName() : "";
}

const DynamicArray<IncomeSource>& IncomeSourceManager::getAllIncomeSources() const { return m_sources; }

int IncomeSourceManager::getActiveCount() const {
    int count = 0;
    for (int i = 0; i < m_sources.getSize(); ++i) {
        if (!m_sources[i].isDeleted()) ++count;
    }

    return count;
}

bool IncomeSourceManager::loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath) {
    (void)recordFilePath; (void)namePoolPath; return false;
}

bool IncomeSourceManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    (void)recordFilePath; (void)namePoolPath; return false;
}
