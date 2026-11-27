#include "core/BinaryIO.h"
#include <fstream>
#include <sys/stat.h>

namespace BinaryIO {
    bool writeBuffer(const std::string& filepath, const void* data, size_t size) {
        std::ofstream ofs(filepath, std::ios::binary | std::ios::trunc);
        if (!ofs) return false;
        ofs.write(reinterpret_cast<const char*>(data), size);
        return true;
    }
    bool readBuffer(const std::string& filepath, void* data, size_t size) {
        std::ifstream ifs(filepath, std::ios::binary);
        if (!ifs) return false;
        ifs.read(reinterpret_cast<char*>(data), size);
        return true;
    }
    bool appendBuffer(const std::string& filepath, const void* data, size_t size) {
        std::ofstream ofs(filepath, std::ios::binary | std::ios::app);
        if (!ofs) return false;
        ofs.write(reinterpret_cast<const char*>(data), size);
        return true;
    }
    size_t getFileSize(const std::string& filepath) {
        struct stat stat_buf;
        int rc = stat(filepath.c_str(), &stat_buf);
        return rc == 0 ? stat_buf.st_size : 0;
    }
    bool fileExists(const std::string& filepath) {
        struct stat stat_buf;
        return (stat(filepath.c_str(), &stat_buf) == 0);
    }
}
