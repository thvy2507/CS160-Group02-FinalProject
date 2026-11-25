/**
 * @file test_binary_io.cpp
 * @author TV1 (Vy)
 * @brief Tests binary serialisation and string pool utilities
 */

#include "core/BinaryIO.h"

#ifdef NDEBUG
#undef NDEBUG
#endif

#include <cassert>
#include <iostream>
#include <limits>
#include <sstream>
#include <string>

using namespace BinaryIO;

// Integer Serialisation Tests
void testIntegerSerialisation() {
    std::stringstream file;

    assert(writeUInt8(file, 255));
    assert(writeUInt32(file, 0x12345678));
    assert(writeInt32(file, -12345));
    assert(writeInt64(file, -1000000));

    assert(file.str().size() == 17);

    file.seekg(0);

    uint8_t a = 0;
    uint32_t b = 0;
    int32_t c = 0;
    int64_t d = 0;

    assert(readUInt8(file, a));
    assert(readUInt32(file, b));
    assert(readInt32(file, c));
    assert(readInt64(file, d));

    assert(a == 255);
    assert(b == 0x12345678);
    assert(c == -12345);
    assert(d == -1000000);
}

void testLittleEndianEncoding() {
    std::ostringstream file;

    assert(writeUInt32(file, 0x12345678));
    assert(writeInt32(file, -2));
    assert(writeInt64(file, -1));

    const std::string expected(
        "\x78\x56\x34\x12"
        "\xFE\xFF\xFF\xFF"
        "\xFF\xFF\xFF\xFF\xFF\xFF\xFF\xFF",
        16
    );

    assert(file.str() == expected);
}

void testIntegerLimits() {
    std::stringstream file;

    const int32_t signed32Values[] = { std::numeric_limits<int32_t>::min(), -1, 0, 1, std::numeric_limits<int32_t>::max() };

    const int64_t signed64Values[] = { std::numeric_limits<int64_t>::min(), -1, 0, 1, std::numeric_limits<int64_t>::max() };

    for (int32_t value : signed32Values) {
        assert(writeInt32(file, value));
    }

    for (int64_t value : signed64Values) {
        assert(writeInt64(file, value));
    }

    assert(writeUInt32(file, std::numeric_limits<uint32_t>::max()));

    file.seekg(0);

    for (int32_t expected : signed32Values) {
        int32_t result = 0;
        assert(readInt32(file, result));
        assert(result == expected);
    }

    for (int64_t expected : signed64Values) {
        int64_t result = 0;
        assert(readInt64(file, result));
        assert(result == expected);
    }

    uint32_t result = 0;

    assert(readUInt32(file, result));
    assert(result == std::numeric_limits<uint32_t>::max());
}

void testFailedIntegerOperations() {
    std::istringstream empty("");
    std::istringstream truncated32("AB");
    std::istringstream truncatedSigned32("AB");
    std::istringstream truncated64("ABCD");

    uint8_t a = 10;
    uint32_t b = 20;
    int32_t c = -30;
    int64_t d = -40;

    assert(!readUInt8(empty, a));
    assert(!readUInt32(truncated32, b));
    assert(!readInt32(truncatedSigned32, c));
    assert(!readInt64(truncated64, d));

    assert(a == 10);
    assert(b == 20);
    assert(c == -30);
    assert(d == -40);

    std::ostringstream invalid;
    invalid.setstate(std::ios::failbit);

    assert(!writeUInt8(invalid, 1));
    assert(!writeUInt32(invalid, 1));
    assert(!writeInt32(invalid, -1));
    assert(!writeInt64(invalid, -1));
}

// Header Operation Tests
void testHeaderSerialization() {
    std::stringstream file;

    assert(writeHeader(file, WALLET_MAGIC, WALLET_RECORD_SIZE, 3));

    assert(file.str() == std::string(
        "WALT\x01\x00\x00\x00"
        "\x1D\x00\x00\x00"
        "\x03\x00\x00\x00",
        16
    ));

    file.seekg(0);

    FileHeader header{};

    assert(readHeader(file, header));

    assert(std::string(header.magic, 4) == WALLET_MAGIC);
    assert(header.version == FORMAT_VERSION);
    assert(header.recordSize == WALLET_RECORD_SIZE);
    assert(header.recordCount == 3);
}

void testHeaderValidation() {
    FileHeader header{
        {'W', 'A', 'L', 'T'},
        FORMAT_VERSION,
        WALLET_RECORD_SIZE,
        3
    };

    assert(validateHeader(header, WALLET_MAGIC, 29, 103, 100));

    // Incorrect magic and record size
    assert(!validateHeader(header, INCOME_SOURCE_MAGIC, 29, 103, 100));
    assert(!validateHeader(header, WALLET_MAGIC, 13, 103, 100));

    // Incorrect file size
    assert(!validateHeader(header, WALLET_MAGIC, 29, 102, 100));
    assert(!validateHeader(header, WALLET_MAGIC, 29, 104, 100));

    // Record count exceeds limit
    assert(!validateHeader(header, WALLET_MAGIC, 29, 103, 2));

    // Unsupported version
    header.version = FORMAT_VERSION + 1;
    assert(!validateHeader(header, WALLET_MAGIC, 29, 103, 100));

    // Empty dataset
    header.version = FORMAT_VERSION;
    header.recordCount = 0;

    assert(validateHeader(header, WALLET_MAGIC, 29, 16, 100));
    assert(!validateHeader(header, WALLET_MAGIC, 29, 0, 100));
}

void testFailedHeaderOperations() {
    std::ostringstream output;

    assert(!writeHeader(output, "BAD", WALLET_RECORD_SIZE, 3));
    assert(!writeHeader(output, WALLET_MAGIC, 0, 3));
    assert(output.str().empty());

    std::istringstream truncated("WALT");

    FileHeader header{
        {'T', 'E', 'S', 'T'},
        7,
        99,
        5
    };

    assert(!readHeader(truncated, header));

    assert(std::string(header.magic, 4) == "TEST");
    assert(header.version == 7);
    assert(header.recordSize == 99);
    assert(header.recordCount == 5);
}

// File Operation Tests
void testFileSize() {
    std::istringstream file("Hello World");

    file.seekg(5);

    uint64_t size = 0;

    assert(getFileSize(file, size));
    assert(size == 11);
    assert(file.tellg() == std::streampos(5));

    std::istringstream empty("");

    assert(getFileSize(empty, size));
    assert(size == 0);

    std::istringstream invalid("ABC");
    invalid.setstate(std::ios::failbit);

    size = 999;

    assert(!getFileSize(invalid, size));
    assert(size == 999);
}

// String Pool Tests
void testStringPoolOperations() {
    std::stringstream pool;

    uint32_t offset = 0;
    uint32_t length = 0;

    assert(appendString(pool, "Cash", offset, length));
    assert(offset == 0 && length == 4);

    assert(appendString(pool, "Bank", offset, length));
    assert(offset == 4 && length == 4);

    // Appending must always write at the end.
    pool.seekp(0);

    assert(appendString(pool, "Savings", offset, length));
    assert(offset == 8 && length == 7);

    assert(pool.str() == "CashBankSavings");

    uint64_t poolSize = 0;
    assert(getFileSize(pool, poolSize));
    assert(poolSize == 15);

    std::string value;

    assert(readString(pool, 0, 4, poolSize, value));
    assert(value == "Cash");

    assert(readString(pool, 4, 4, poolSize, value));
    assert(value == "Bank");

    assert(readString(pool, 8, 7, poolSize, value));
    assert(value == "Savings");
}

void testEmptyAndUtf8Strings() {
    std::stringstream pool;

    uint32_t offset = 0;
    uint32_t length = 0;
    std::string value = "Old value";

    assert(appendString(pool, "", offset, length));
    assert(offset == 0 && length == 0);

    assert(readString(pool, offset, length, 0, value));
    assert(value.empty());

    const std::string name = u8"Ví";

    assert(appendString(pool, name, offset, length));
    assert(offset == 0 && length == 3);

    assert(readString(pool, offset, length, 3, value));
    assert(value == name);

    assert(appendString(pool, "", offset, length));
    assert(offset == 3 && length == 0);

    assert(readString(pool, offset, length, 3, value));
    assert(value.empty());
}

void testPoolReferenceValidation() {
    constexpr uint32_t MAX_POOL_SIZE = std::numeric_limits<uint32_t>::max();

    assert(isValidPoolReference(0, 0, 0));
    assert(isValidPoolReference(0, 4, 15));
    assert(isValidPoolReference(8, 7, 15));
    assert(isValidPoolReference(15, 0, 15));

    assert(!isValidPoolReference(16, 0, 15));
    assert(!isValidPoolReference(15, 1, 15));
    assert(!isValidPoolReference(8, 10, 15));

    assert(isValidPoolReference(MAX_POOL_SIZE, 0, MAX_POOL_SIZE));
    assert(!isValidPoolReference(MAX_POOL_SIZE, 1, MAX_POOL_SIZE));

    assert(!isValidPoolReference(0, 0, static_cast<uint64_t>(MAX_POOL_SIZE) + 1));
}

void testFailedStringOperations() {
    std::istringstream pool("CashBankSavings");

    std::string value = "Unchanged";

    assert(!readString(pool, 8, 100, 15, value));
    assert(value == "Unchanged");

    std::istringstream truncated("AB");

    assert(!readString(truncated, 0, 4, 4, value));
    assert(value == "Unchanged");

    std::ostringstream invalid;
    invalid.setstate(std::ios::failbit);

    uint32_t offset = 99;
    uint32_t length = 88;

    assert(!appendString(invalid, "Test", offset, length));
    assert(offset == 99);
    assert(length == 88);
}

int main() {
    testIntegerSerialisation();
    testLittleEndianEncoding();
    testIntegerLimits();
    testFailedIntegerOperations();

    testHeaderSerialization();
    testHeaderValidation();
    testFailedHeaderOperations();

    testFileSize();

    testStringPoolOperations();
    testEmptyAndUtf8Strings();
    testPoolReferenceValidation();
    testFailedStringOperations();

    std::cout << "All 12 Binary I/O test groups passed!\n";

    return 0;
}
