//
// Created by turiing on 2026/1/14.
//

#ifndef HYPERENGINE_COMMANDPOOL_H
#define HYPERENGINE_COMMANDPOOL_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN

class CommandPool: public VulObject<VkCommandPool> {
public:
    CommandPool();
    ~CommandPool();
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_COMMANDPOOL_H