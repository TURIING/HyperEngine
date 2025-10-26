//
// Created by turiing on 2025/9/27.
//

#ifndef RESOURCE_H
#define RESOURCE_H

#include "../common/Common.h"

USING_ENGINE_NAMESPACE_BEGIN

class ENGINE_EXPORT Resource : NoCopyable {
public:
    Resource() = default;
    NODISCARD virtual std::type_index GetTypeIndex() const = 0;
};

USING_ENGINE_NAMESPACE_END
#endif //RESOURCE_H
