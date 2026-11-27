#ifndef CORE_BINARY_IO_H
#define CORE_BINARY_IO_H

#include <string>
#include <vector>

// Mock cua TV1 de dung thu trong Sprint 3 (xu ly Doc/Ghi File nhi phan)
namespace BinaryIO {
    bool writeBuffer(const std::string& filepath, const void* data, size_t size);
    bool readBuffer(const std::string& filepath, void* data, size_t size);
    bool appendBuffer(const std::string& filepath, const void* data, size_t size);
    size_t getFileSize(const std::string& filepath);
    bool fileExists(const std::string& filepath);
}

#endif
