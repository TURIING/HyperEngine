//
// Created by turiing on 2025/10/7.
//

#ifndef FSTREAM_HPP
#define FSTREAM_HPP

#include "../common/Common.h"
#include "BaseFStream.hpp"
#include "FBuffer.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class FStream: public BaseFStream, public std::iostream {
public:
    FStream(const std::filesystem::path &filename, FileMode mode):
        BaseFStream(sOpenWithMode(filename, mode)),
        std::iostream(new FBuffer(m_pFile)) {

    }

    ~FStream() override {
        delete rdbuf();
    }
};
USING_ENGINE_NAMESPACE_END
#endif //FSTREAM_HPP
