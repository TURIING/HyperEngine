//
// Created by turiing on 2026/1/28.
//

#ifndef HYPERENGINE_CMDMANAGER_H
#define HYPERENGINE_CMDMANAGER_H
#include "../../common/Common.h"
#include "../device/Queue.h"

USING_ENGINE_NAMESPACE_BEGIN
class CommandPool;
class CommandBuffer;

class CmdManager final {
public:
    CmdManager();

    NODISCARD CommandBuffer* CreateCommandBuffer() const;
	void WithSingleCmdBuffer(const std::function<void(CommandBuffer* pCmd)>& func) const;

private:
	Share<CommandPool> m_pCmdPool;
	Unique<Queue> m_pQueue;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_CMDMANAGER_H