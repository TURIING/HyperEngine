//
// Created by turiing on 2026/2/3.
//

#ifndef HYPERENGINE_SEMAPHORE_H
#define HYPERENGINE_SEMAPHORE_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN

class Semaphore: public VulObject<VkSemaphore> {
public:
    explicit Semaphore();
    ~Semaphore() override;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_SEMAPHORE_H
