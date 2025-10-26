//
// Created by turiing on 2025/10/7.
//

#ifndef IFSTREAM_HPP
#define IFSTREAM_HPP

#include "../common/Common.h"
#include "BaseFStream.hpp"
#include "FBuffer.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class IFStream: public BaseFStream, public std::istream {
public:
    explicit IFStream(const std::filesystem::path &filename):
        BaseFStream(sOpenWithMode(filename, FileMode::Read)),
        std::istream(new FBuffer(m_pFile)) {

    }
    ~IFStream() override {
        delete rdbuf();
    }
};
USING_ENGINE_NAMESPACE_END
#endif //IFSTREAM_HPP
