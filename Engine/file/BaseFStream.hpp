//
// Created by turiing on 2025/10/7.
//

#ifndef BASEFSTREAM_HPP
#define BASEFSTREAM_HPP

#include "../common/Common.h"
#include "physfs.h"

USING_ENGINE_NAMESPACE_BEGIN
enum class FileMode {
    Read, Write, Append
};

class ENGINE_EXPORT BaseFStream {
public:
    explicit BaseFStream(PHYSFS_File* file): m_pFile(file) {
        LOG_ASSERT(file);
    }
    virtual ~BaseFStream() {
        PHYSFS_close(m_pFile);
    }
    NODISCARD size_t Length() const {
        return PHYSFS_fileLength(m_pFile);
    }

    NODISCARD static PHYSFS_File* sOpenWithMode(const std::filesystem::path &filename, FileMode mode) {
        PHYSFS_File* file = nullptr;

        auto pathStr = filename.string();
        std::replace(pathStr.begin(), pathStr.end(), '\\', '/');

        switch (mode) {
            case FileMode::Read:
                file = PHYSFS_openRead(pathStr.c_str());
                break;
            case FileMode::Write:
                file = PHYSFS_openWrite(pathStr.c_str());
                break;
            case FileMode::Append:
                file = PHYSFS_openAppend(pathStr.c_str());
                break;
        }

        LOG_ASSERT(file);
        return file;
    }

protected:
    PHYSFS_File* m_pFile = nullptr;
};
USING_ENGINE_NAMESPACE_END
#endif //BASEFSTREAM_HPP
