//
// Created by turiing on 2026/2/3.
//

#ifndef HYPERENGINE_FENCE_H
#define HYPERENGINE_FENCE_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN

class Fence: public VulObject<VkFence> {
public:
    explicit Fence(bool signaled = false);
    ~Fence() override;
    WaitState Wait(uint64_t timeout = UINT64_MAX) const;
    void Reset() const;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_FENCE_H
