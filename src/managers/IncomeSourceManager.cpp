#include "managers/IncomeSourceManager.h"
#include <iostream>

IncomeSourceManager::IncomeSourceManager() : sources_(), nextId_(1) {}

int IncomeSourceManager::addIncomeSource(const std::string& name) {
    if (name.empty() || findIndexByName(name) != -1) return IncomeSource::INVALID_ID;
    int assignedId = nextId_++;
    IncomeSource newSource(assignedId, name);
    sources_.pushBack(newSource);
    return assignedId;
}

bool IncomeSourceManager::editIncomeSource(int sourceId, const std::string& newName) {
    if (newName.empty()) return false;
    int targetIdx = findIndexById(sourceId);
    if (targetIdx == -1) return false;
    int duplicateIdx = findIndexByName(newName);
    if (duplicateIdx != -1 && duplicateIdx != targetIdx) return false;
    return sources_[targetIdx].setName(newName);
}

bool IncomeSourceManager::deleteIncomeSource(int sourceId) {
    int targetIdx = findIndexById(sourceId);
    if (targetIdx == -1) return false;
    sources_[targetIdx].setDeleted(true);
    return true;
}

void IncomeSourceManager::viewAllIncomeSources() const {
    std::cout << "\n=== DANH SACH NGUON THU ===\n";
    int activeCount = 0;
    for (int i = 0; i < sources_.getSize(); ++i) {
        if (!sources_[i].isDeleted()) {
            ++activeCount;
            std::cout << "ID: " << sources_[i].getId() << " | Ten nguon thu: " << sources_[i].getName() << "\n";
        }
    }
    if (activeCount == 0) std::cout << "(Chua co nguon thu nao dang hoat dong)\n";
}

bool IncomeSourceManager::exists(int sourceId) const { return findIndexById(sourceId) != -1; }

int IncomeSourceManager::findIndexById(int sourceId) const {
    if (sourceId <= IncomeSource::INVALID_ID) return -1;
    for (int i = 0; i < sources_.getSize(); ++i) {
        if (!sources_[i].isDeleted() && sources_[i].getId() == sourceId) return i;
    }
    return -1;
}

int IncomeSourceManager::findIndexByName(const std::string& name) const {
    if (name.empty()) return -1;
    for (int i = 0; i < sources_.getSize(); ++i) {
        if (!sources_[i].isDeleted() && sources_[i].getName() == name) return i;
    }
    return -1;
}

IncomeSource* IncomeSourceManager::getIncomeSourceById(int sourceId) {
    int idx = findIndexById(sourceId);
    return (idx != -1) ? &sources_[idx] : nullptr;
}

const IncomeSource* IncomeSourceManager::getIncomeSourceById(int sourceId) const {
    int idx = findIndexById(sourceId);
    return (idx != -1) ? &sources_[idx] : nullptr;
}

std::string IncomeSourceManager::getIncomeSourceName(int sourceId) const {
    const IncomeSource* source = getIncomeSourceById(sourceId);
    return (source != nullptr) ? source->getName() : "";
}

const DynamicArray<IncomeSource>& IncomeSourceManager::getAllIncomeSources() const { return sources_; }

int IncomeSourceManager::getActiveCount() const {
    int count = 0;
    for (int i = 0; i < sources_.getSize(); ++i) {
        if (!sources_[i].isDeleted()) ++count;
    }
    return count;
}

bool IncomeSourceManager::loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath) {
    (void)recordFilePath; (void)namePoolPath; return false;
}
bool IncomeSourceManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    (void)recordFilePath; (void)namePoolPath; return false;
}
