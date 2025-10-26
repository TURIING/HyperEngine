//
// Created by turiing on 2025/9/21.
//

#ifndef NOCOPYABLE_H
#define NOCOPYABLE_H

#include "BaseDefine.h"
#include "Export.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class ENGINE_EXPORT NoCopyable {
protected:
    NoCopyable() = default;
    virtual ~NoCopyable() = default;

public:
    NoCopyable(const NoCopyable &) = delete;
    NoCopyable(NoCopyable &&) noexcept = delete;
    NoCopyable &operator=(const NoCopyable &) = delete;
    NoCopyable &operator=(NoCopyable &&) noexcept = default;
};

USING_ENGINE_NAMESPACE_END
#endif //NOCOPYABLE_H
