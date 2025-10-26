//
// Created by turiing on 2025/10/7.
//

#ifndef OFSTREAM_HPP
#define OFSTREAM_HPP

#include "../common/Common.h"
#include "BaseFStream.hpp"
#include "FBuffer.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class OFStream: public BaseFStream, public std::ostream {
public:
    explicit OFStream(const std::filesystem::path &filename, FileMode mode):
        BaseFStream(sOpenWithMode(filename, mode)),
        std::ostream(new FBuffer(m_pFile)) {

    }

    ~OFStream() override {
        delete rdbuf();
    }
};
USING_ENGINE_NAMESPACE_END
#endif //OFSTREAM_HPP
