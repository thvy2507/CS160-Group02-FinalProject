/**
 * @file BinaryIO.cpp
 * @author TV1 (Vy)
 * @brief Inplements shared binary serialisation utilities.
 * @note Uses the version 1 little-endian on-disk format.
 */

#include "core/BinaryIO.h"

#include <cstring>
#include <istream>
#include <limits>
#include <ostream>

namespace {

    // Constants
    constexpr int BITS_PER_BYTE = 8;
    constexpr uint64_t BYTE_MASK = 0xFF;

    static_assert(std::numeric_limits<std::streamoff>::digits >= std::numeric_limits<uint32_t>::digits, "Stream offset must support 32-bit pool offsets.");

    static_assert(std::numeric_limits<std::streamsize>::digits >= std::numeric_limits<uint32_t>::digits, "Stream size must support 32-bit pool lengths.");

    // Integer Serialisation Helpers
    bool writeUnsigned(std::ostream& file, uint64_t value, int byteCount) {
        unsigned char bytes[sizeof(uint64_t)]{};

        for (int i = 0; i < byteCount; ++i) {
            bytes[i] = static_cast<unsigned char>((value >> (i * BITS_PER_BYTE)) & BYTE_MASK);
        }

        file.write(reinterpret_cast<const char*>(bytes), byteCount);

        return static_cast<bool>(file);
    }

    bool readUnsigned(std::istream& file, uint64_t& value, int byteCount) {
        unsigned char bytes[sizeof(uint64_t)]{};

        if (!file.read(reinterpret_cast<char*>(bytes), byteCount)) {
            return false;
        }

        uint64_t result = 0;
        for (int i = 0; i < byteCount; ++i) {
            result |= static_cast<uint64_t>(bytes[i]) << (i * BITS_PER_BYTE);
        }

        value = result;
        return true;
    }

} // namespace

namespace BinaryIO {

    // Integer Write Operations
    bool writeUInt8(std::ostream& file, uint8_t value) {
        return writeUnsigned(file, value, sizeof(value));
    }

    bool writeUInt32(std::ostream& file, uint32_t value) {
        return writeUnsigned(file, value, sizeof(value));
    }

    bool writeInt32(std::ostream& file, int32_t value) {
        return writeUInt32(file, static_cast<uint32_t>(value));
    }

    bool writeInt64(std::ostream& file, int64_t value) {
        return writeUnsigned(file, static_cast<uint64_t>(value), sizeof(value));
    }

    // Integer Read Operations
    bool readUInt8(std::istream& file, uint8_t& value) {
        uint64_t result = 0;

        if (!readUnsigned(file, result, sizeof(value))) {
            return false;
        }

        value = static_cast<uint8_t>(result);
        return true;
    }

    bool readUInt32(std::istream& file, uint32_t& value) {
        uint64_t result = 0;

        if (!readUnsigned(file, result, sizeof(value))) {
            return false;
        }

        value = static_cast<uint32_t>(result);
        return true;
    }

    bool readInt32(std::istream& file, int32_t& value) {
        uint32_t result = 0;

        if (!readUInt32(file, result)) {
            return false;
        }

        if (result <= std::numeric_limits<int32_t>::max()) {
            value = static_cast<int32_t>(result);
        } else {
            value = -1 - static_cast<int32_t>(std::numeric_limits<uint32_t>::max() - result);
        }

        return true;
    }

    bool readInt64(std::istream& file, int64_t& value) {
        uint64_t result = 0;

        if (!readUnsigned(file, result, sizeof(value))) {
            return false;
        }

        if (result <= std::numeric_limits<int64_t>::max()) {
            value = static_cast<int64_t>(result);
        } else {
            value = -1 - static_cast<int64_t>(std::numeric_limits<uint64_t>::max() - result);
        }

        return true;
    }

    // Header Operations
    bool writeHeader(std::ostream& file, const std::string& magic, uint32_t recordSize, uint32_t recordCount) {
        if (magic.size() != 4 || recordSize == 0) {
            return false;
        }

        if (!file.write(magic.data(), 4)) {
            return false;
        }

        return writeUInt32(file, FORMAT_VERSION) && writeUInt32(file, recordSize) && writeUInt32(file, recordCount);
    }

    bool readHeader(std::istream& file, FileHeader& header) {
        FileHeader result{};

        if (!file.read(result.magic, sizeof(result.magic)) || !readUInt32(file, result.version) || !readUInt32(file, result.recordSize) || !readUInt32(file, result.recordCount)) {
            return false;
        }

        header = result;
        return true;
    }

    bool validateHeader(const FileHeader& header, const std::string& expectedMagic, uint32_t expectedRecordSize, uint64_t fileSize, uint32_t maxRecordCount) {
        if (expectedMagic.size() != sizeof(header.magic) || expectedRecordSize == 0) {
            return false;
        }

        if (std::memcmp(header.magic, expectedMagic.data(), sizeof(header.magic)) != 0) {
            return false;
        }

        if (header.version != FORMAT_VERSION || header.recordSize != expectedRecordSize || header.recordCount > maxRecordCount) {
            return false;
        }

        uint64_t expectedFileSize = HEADER_SIZE + static_cast<uint64_t>(header.recordCount) * header.recordSize;

        return fileSize == expectedFileSize;
    }

    // File Operations
    bool getFileSize(std::istream& file, uint64_t& fileSize) {
        std::streampos originalPosition = file.tellg();

        if (originalPosition == std::streampos(-1)) {
            return false;
        }

        file.seekg(0, std::ios::end);
        std::streampos endPosition = file.tellg();

        if (!file || endPosition == std::streampos(-1)) {
            return false;
        }

        std::streamoff size = endPosition - std::streampos(0);

        if (size < 0) {
            return false;
        }

        file.seekg(originalPosition);

        if (!file) {
            return false;
        }

        fileSize = static_cast<uint64_t>(size);
        return true;
    }

    // String Pool Operations
    bool isValidPoolReference(uint32_t offset, uint32_t length, uint64_t poolSize) {
        if (poolSize > std::numeric_limits<uint32_t>::max()) {
            return false;
        }

        if (offset > poolSize) {
            return false;
        }

        return length <= poolSize - offset;
    }

    bool appendString(std::ostream& file, const std::string& value, uint32_t& offset, uint32_t& length) {
        constexpr uint64_t MAX_POOL_SIZE = std::numeric_limits<uint32_t>::max();

        if (!file || value.size() > MAX_POOL_SIZE) {
            return false;
        }

        file.seekp(0, std::ios::end);

        if (!file) {
            return false;
        }

        std::streampos position = file.tellp();
        if (position == std::streampos(-1)) {
            return false;
        }

        std::streamoff currentSize = position - std::streampos(0);

        if (currentSize < 0) {
            return false;
        }

        uint64_t poolSize = static_cast<uint64_t>(currentSize);

        if (poolSize > MAX_POOL_SIZE || value.size() > MAX_POOL_SIZE - poolSize) {
            return false;
        }

        if (!value.empty() && !file.write(value.data(), static_cast<std::streamsize>(value.size()))) {
            return false;
        }

        offset = static_cast<uint32_t>(poolSize);
        length = static_cast<uint32_t>(value.size());

        return true;
    }

    bool readString(std::istream& file, uint32_t offset, uint32_t length, uint64_t poolSize, std::string& value) {
        if (!file || !isValidPoolReference(offset, length, poolSize)) {
            return false;
        }

        if (length > value.max_size()) {
            return false;
        }

        file.seekg(static_cast<std::streamoff>(offset), std::ios::beg);

        if (!file) {
            return false;
        }

        std::string result(length, '\0');

        if (length != 0 && !file.read(result.data(), static_cast<std::streamsize>(length))) {
            return false;
        }

        value.swap(result);

        return true;
    }

} // namespace BinaryIO
