//
// Created by turiing on 2025/10/1.
//

#ifndef FBUFFER_HPP
#define FBUFFER_HPP

#include "../common/Common.h"
#include "physfs.h"

USING_ENGINE_NAMESPACE_BEGIN

class FBuffer final: public std::streambuf, NoCopyable {
public:
    explicit FBuffer(PHYSFS_File *file, std::size_t bufferSize = 2048): m_bufferSize(bufferSize), m_pFile(file) {
        m_pBuffer = new char[bufferSize];
        auto pEnd = m_pBuffer + bufferSize;
        std::streambuf::setg(pEnd, pEnd, pEnd);
        std::streambuf::setp(m_pBuffer, pEnd);
    }

    ~FBuffer() override {
        sync();
        delete[] m_pBuffer;
    }

protected:
    pos_type seekoff(off_type pos, std::ios_base::seekdir dir, std::ios_base::openmode mode) override {
        switch (dir) {
            case std::ios_base::beg: {
                PHYSFS_seek(m_pFile, pos);
                break;
            }
            case std::ios_base::cur: {
                PHYSFS_seek(m_pFile, (PHYSFS_tell(m_pFile) + pos) - (egptr() - gptr()));
                break;
            }
            case std::ios_base::end: {
                PHYSFS_seek(m_pFile, PHYSFS_fileLength(m_pFile) + pos);
                break;
            }
        }

        if (mode & std::ios_base::in) {
            setg(egptr(), egptr(), egptr());
        }
        if (mode & std::ios_base::out) {
            setp(m_pBuffer, m_pBuffer);
        }
        return PHYSFS_tell(m_pFile);
    }

    pos_type seekpos(pos_type pos, std::ios_base::openmode mode) override {
        PHYSFS_seek(m_pFile, pos);

        if (mode & std::ios_base::in) {
            setg(egptr(), egptr(), egptr());
        }
        if (mode & std::ios_base::out) {
            setp(m_pBuffer, m_pBuffer);
        }

        return PHYSFS_tell(m_pFile);
    }

    int sync() override {
        return overflow();
    }

    int_type overflow(int_type c = traits_type::eof()) override {
        if (pptr() == pbase() && c == traits_type::eof()) {
            return 0;
        }

        if (PHYSFS_writeBytes(m_pFile, pbase(), static_cast<PHYSFS_uint32>(pptr() - pbase())) < 1) {
            return traits_type::eof();
        }

        if (c != traits_type::eof() && PHYSFS_writeBytes(m_pFile, &c, 1) < 1) {
            return traits_type::eof();
        }

        return 0;
    }

    int_type underflow() override {
        if (PHYSFS_eof(m_pFile)) {
            return traits_type::eof();
        }

        auto bytesRead = PHYSFS_readBytes(m_pFile, m_pBuffer, static_cast<PHYSFS_uint32>(m_bufferSize));
        if (bytesRead < 1) {
            return traits_type::eof();
        }

        setg(m_pBuffer, m_pBuffer, m_pBuffer + bytesRead);
        return static_cast<int_type>(*gptr());
    }


private:
    char *m_pBuffer = nullptr;
    size_t m_bufferSize;
    PHYSFS_File *m_pFile = nullptr;
};
USING_ENGINE_NAMESPACE_END
#endif //FBUFFER_HPP
