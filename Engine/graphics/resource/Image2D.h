//
// Created by turiing on 2025/9/29.
//

#ifndef IMAGE2D_H
#define IMAGE2D_H

#include "../../common/Common.h"
#include "../../common/Node.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class Image2D {
public:
    static std::shared_ptr<Image2D> Create(const Node &node);
};

USING_ENGINE_NAMESPACE_END

#endif //IMAGE2D_H
