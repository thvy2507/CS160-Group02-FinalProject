/**
 * @file IncomeSourceManager.cpp
 * @author TV2 (Khoi)
 * @brief Implements income source management operations.
 */

#include "managers/IncomeSourceManager.h"

#include <fstream>
#include <iostream>
#include <limits>

#include "core/BinaryIO.h"

namespace {

    bool writeIncomeSourceRecord(std::ostream& file, const IncomeSourceRecord& record) {
        return BinaryIO::writeInt32(file, record.id)
            && BinaryIO::writeUInt32(file, record.nameOffset)
            && BinaryIO::writeUInt32(file, record.nameLength)
            && BinaryIO::writeUInt8(file, record.isDeleted);
    }

    bool readIncomeSourceRecord(std::istream& file, IncomeSourceRecord& record) {
        return BinaryIO::readInt32(file, record.id)
            && BinaryIO::readUInt32(file, record.nameOffset)
            && BinaryIO::readUInt32(file, record.nameLength)
            && BinaryIO::readUInt8(file, record.isDeleted);
    }

    bool isValidIncomeSourceRecord(
        const IncomeSourceRecord& record,
        const std::string& name,
        const DynamicArray<IncomeSource>& sources) {

        if (record.id <= IncomeSource::INVALID_ID ||
            record.id == std::numeric_limits<int32_t>::max() ||
            record.isDeleted > 1 ||
            name.empty()) {
            return false;
        }

        for (int i = 0; i < sources.size(); ++i) {
            const IncomeSource& source = sources[i];

            if (source.getId() == record.id) {
                return false;
            }

            if (record.isDeleted == 0 &&
                !source.isDeleted() &&
                source.getName() == name) {
                return false;
            }
        }

        return true;
    }

} // namespace

IncomeSourceManager::IncomeSourceManager() : m_sources(), m_nextId(1) {}

int IncomeSourceManager::addIncomeSource(const std::string& name) {
    if (m_nextId == std::numeric_limits<int32_t>::max() || name.empty() || findIndexByName(name) != -1) {
        return IncomeSource::INVALID_ID;
    }

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
    if (recordFilePath == namePoolPath) {
        return false;
    }

    std::ifstream recordFile(recordFilePath, std::ios::binary);
    std::ifstream namePool(namePoolPath, std::ios::binary);
    if (!recordFile || !namePool) {
        return false;
    }

    uint64_t recordFileSize, poolSize;
    BinaryIO::FileHeader header{};
    if (!BinaryIO::getFileSize(recordFile, recordFileSize) ||
        !BinaryIO::getFileSize(namePool, poolSize) ||
        !BinaryIO::readHeader(recordFile, header) ||
        !BinaryIO::validateHeader(
            header, BinaryIO::INCOME_SOURCE_MAGIC,
            BinaryIO::INCOME_SOURCE_RECORD_SIZE, recordFileSize,
            std::numeric_limits<int>::max() - 1) ||
        poolSize > std::numeric_limits<uint32_t>::max()) {
        return false;
    }

    DynamicArray<IncomeSource> tempSources(header.recordCount == 0 ?                                     4 : static_cast<int>(header.recordCount)
                                           );

    int maxId = 0;
    for (uint32_t i = 0; i < header.recordCount; ++i) {
        IncomeSourceRecord record{};
        std::string name;

        if (!readIncomeSourceRecord(recordFile, record) ||
            !BinaryIO::readString(namePool, record.nameOffset, record.nameLength, poolSize, name) ||
            !isValidIncomeSourceRecord(record, name, tempSources)) {
            return false;
        }

        tempSources.add(IncomeSource::fromRecord(record, name));

        if (record.id > maxId) {
            maxId = record.id;
        }
    }

    m_sources = tempSources;
    m_nextId = maxId + 1;

    return true;
}

bool IncomeSourceManager::saveToBinary(const std::string& recordFilePath, const std::string& namePoolPath) const {
    if (recordFilePath == namePoolPath) {
        return false;
    }

    std::ofstream recordFile(recordFilePath, std::ios::binary | std::ios::trunc);
    std::ofstream namePool(namePoolPath, std::ios::binary | std::ios::trunc);

    if (!recordFile || !namePool) {
        return false;
    }

    uint32_t recordCount = static_cast<uint32_t>(m_sources.size());
    if (!BinaryIO::writeHeader(recordFile, BinaryIO::INCOME_SOURCE_MAGIC, BinaryIO::INCOME_SOURCE_RECORD_SIZE, recordCount)) {
        return false;
    }

    for (int i = 0; i < m_sources.size(); ++i) {
        const IncomeSource& source = m_sources[i];

        uint32_t offset, length;

        if (!BinaryIO::appendString(namePool, source.getName(), offset, length)) {
            return false;
        }

        IncomeSourceRecord record = source.toRecord(offset, length);
        if (!writeIncomeSourceRecord(recordFile, record)) {
            return false;
        }
    }

    recordFile.close();
    namePool.close();

    return static_cast<bool>(recordFile) && static_cast<bool>(namePool);
}
