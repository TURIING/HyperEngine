//
// Created by turiing on 2026/1/28.
//

#include "CmdManager.h"
#include "CommandPool.h"
#include "CommandBuffer.h"
#include "../device/Queue.h"
#include "../Graphics.h"
#include "../device/PhysicalDevice.h"

USING_ENGINE_NAMESPACE_BEGIN
CmdManager::CmdManager() {
    m_pCmdPool = std::make_shared<CommandPool>();
    m_pQueue = std::make_unique<Queue>(Graphics::Get()->GetPhysicalDevice()->GetQueueFamily(QueueType::Graphics));
    LOG_DEBUG("CmdManager Created");
}

CommandBuffer* CmdManager::CreateCommandBuffer() const {
    return new CommandBuffer(m_pCmdPool);
}

void CmdManager::WithSingleCmdBuffer(const std::function<void(CommandBuffer *pCmd)> &func) const {
    const auto cmd = new CommandBuffer(m_pCmdPool);
    cmd->Begin(true);
    func(cmd);
    cmd->End();

    m_pQueue->Submit({
        .pCmd = static_cast<CommandBuffer *[]>(cmd),
        .cmdCount = 1,
    });
    m_pQueue->Wait();
}

USING_ENGINE_NAMESPACE_END
