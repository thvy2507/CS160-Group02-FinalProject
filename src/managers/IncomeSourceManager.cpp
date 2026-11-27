#include "managers/IncomeSourceManager.h"
#include "core/BinaryIO.h"
#include <iostream>
#include <vector>

IncomeSourceManager::IncomeSourceManager() : sources_(), nextId_(1) {}

int IncomeSourceManager::addIncomeSource(const std::string& name) {
    if (name.empty() || findIndexByName(name) != -1) return IncomeSource::INVALID_ID;
    int assignedId = nextId_++;
    sources_.pushBack(IncomeSource(assignedId, name));
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
    for (int i = 0; i < sources_.getSize(); ++i) {
        if (!sources_[i].isDeleted()) std::cout << "ID: " << sources_[i].getId() << " | Name: " << sources_[i].getName() << "\n";
    }
}
bool IncomeSourceManager::exists(int sourceId) const { return findIndexById(sourceId) != -1; }
int IncomeSourceManager::findIndexById(int sourceId) const {
    for (int i = 0; i < sources_.getSize(); ++i) {
        if (!sources_[i].isDeleted() && sources_[i].getId() == sourceId) return i;
    }
    return -1;
}
int IncomeSourceManager::findIndexByName(const std::string& name) const {
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
    const IncomeSource* src = getIncomeSourceById(sourceId);
    return src ? src->getName() : "";
}
const DynamicArray<IncomeSource>& IncomeSourceManager::getAllIncomeSources() const { return sources_; }
int IncomeSourceManager::getActiveCount() const {
    int c = 0;
    for (int i = 0; i < sources_.getSize(); ++i) if (!sources_[i].isDeleted()) c++;
    return c;
}

bool IncomeSourceManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    std::string namePool = "";
    std::vector<IncomeSourceRecord> records;
    for (int i = 0; i < sources_.getSize(); ++i) {
        const IncomeSource& src = sources_[i];
        uint32_t offset = static_cast<uint32_t>(namePool.size());
        uint32_t len = static_cast<uint32_t>(src.getName().size());
        namePool += src.getName();
        records.push_back(src.toRecord(offset, len));
    }
    if (!records.empty()) {
        if (!BinaryIO::writeBuffer(recordFilePath, records.data(), records.size() * sizeof(IncomeSourceRecord))) return false;
    } else { BinaryIO::writeBuffer(recordFilePath, "", 0); }
    if (!namePool.empty()) {
        if (!BinaryIO::writeBuffer(namePoolPath, namePool.data(), namePool.size())) return false;
    } else { BinaryIO::writeBuffer(namePoolPath, "", 0); }
    return true;
}

bool IncomeSourceManager::loadFromBinary(const std::string& recordFilePath, const std::string& namePoolPath) {
    if (!BinaryIO::fileExists(recordFilePath) || !BinaryIO::fileExists(namePoolPath)) return true;
    size_t recordSize = BinaryIO::getFileSize(recordFilePath);
    size_t poolSize = BinaryIO::getFileSize(namePoolPath);
    if (recordSize % sizeof(IncomeSourceRecord) != 0) return false;
    
    std::vector<IncomeSourceRecord> records(recordSize / sizeof(IncomeSourceRecord));
    std::string pool(poolSize, '\0');
    
    if (recordSize > 0 && !BinaryIO::readBuffer(recordFilePath, records.data(), recordSize)) return false;
    if (poolSize > 0 && !BinaryIO::readBuffer(namePoolPath, pool.data(), poolSize)) return false;

    DynamicArray<IncomeSource> tempSrcs;
    int maxId = 0;
    for (const auto& rec : records) {
        if (rec.nameOffset + rec.nameLength > poolSize) return false;
        std::string name = pool.substr(rec.nameOffset, rec.nameLength);
        tempSrcs.pushBack(IncomeSource::fromRecord(rec, name));
        if (rec.id > maxId) maxId = rec.id;
    }
    sources_ = tempSrcs;
    nextId_ = maxId + 1;
    return true;
}
