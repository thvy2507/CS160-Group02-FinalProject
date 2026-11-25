/**
 * @file BinaryIO.h
 * @author TV1 (Vy)
 * @brief Provides shared binary serialisation and string pool utilities
 * @note Uses the version 1 little-endian on-disk format
 */

#pragma once

#include <cstdint>
#include <iosfwd>
#include <string>

namespace BinaryIO {

    // Format Constants
    constexpr uint32_t FORMAT_VERSION = 1;
    constexpr uint32_t HEADER_SIZE = 16;

    constexpr uint32_t WALLET_RECORD_SIZE = 29;
    constexpr uint32_t INCOME_SOURCE_RECORD_SIZE = 13;
    constexpr uint32_t EXPENSE_CATEGORY_RECORD_SIZE = 13;
    constexpr uint32_t TRANSACTION_RECORD_SIZE = 41;
    constexpr uint32_t RECURRING_RECORD_SIZE = 72;

    constexpr char WALLET_MAGIC[] = "WALT";
    constexpr char INCOME_SOURCE_MAGIC[] = "SRCE";
    constexpr char EXPENSE_CATEGORY_MAGIC[] = "CATG";
    constexpr char TRANSACTION_MAGIC[] = "TRAN";
    constexpr char RECURRING_MAGIC[] = "RECR";

    // File Header
    struct FileHeader {
        char magic[4];
        uint32_t version;
        uint32_t recordSize;
        uint32_t recordCount;
    };

    // Integer Write Operations
    bool writeUInt8(std::ostream& file, uint8_t value);
    bool writeUInt32(std::ostream& file, uint32_t value);
    bool writeInt32(std::ostream& file, int32_t value);
    bool writeInt64(std::ostream& file, int64_t value);

    // Integer Read Operations
    bool readUInt8(std::istream& file, uint8_t& value);
    bool readUInt32(std::istream& file, uint32_t& value);
    bool readInt32(std::istream& file, int32_t& value);
    bool readInt64(std::istream& file, int64_t& value);

    // Header Operations
    bool writeHeader(std::ostream& file, const std::string& magic, uint32_t recordSize, uint32_t recordCount);

    bool readHeader(std::istream& file, FileHeader& header);

    bool validateHeader(const FileHeader& header, const std::string& expectedMagic, uint32_t expectedRecordSize, uint64_t fileSize, uint32_t maxRecordCount);

    // File Operations
    bool getFileSize(std::istream& file, uint64_t& fileSize);

    // String Pool Operations
    bool isValidPoolReference(uint32_t offset, uint32_t length, uint64_t poolSize);

    bool appendString(std::ostream& file, const std::string& value, uint32_t& offset, uint32_t& length);

    bool readString(std::istream& file, uint32_t offset, uint32_t length, uint64_t poolSize, std::string& value);

} // namespace BinaryIO
