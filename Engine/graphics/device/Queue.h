//
// Created by turiing on 2026/1/28.
//

#ifndef HYPERENGINE_QUEUE_H
#define HYPERENGINE_QUEUE_H
#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN
class SwapChain;
class LogicDevice;
class Semaphore;
class Fence;
class CommandBuffer;

struct QueueSubmitInfo {
    Semaphore** pWaitSemaphores = nullptr;
    u32 waitSemaphoreCount = 0;
    Semaphore** pSignalSemaphores = nullptr;
    u32 signalSemaphoreCount = 0;
    Fence* pFence = nullptr;
    CommandBuffer** pCmd = nullptr;
    u32 cmdCount = 0;
    VkPipelineStageFlags waitDstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
};

struct QueuePresentInfo {
    SwapChain** pSwapChain = nullptr;
    u32 swapChainCount = 0;
    u32* imageIndex = 0;
    Semaphore** pWaitSemaphores = nullptr;
    u32 waitSemaphoreCount = 0;
};

class Queue : public VulObject<VkQueue> {
public:
    explicit Queue(u32 queueFamily);
    void Submit(const QueueSubmitInfo &vulSubmitInfo) const;
    void Wait() const;
    void Present(const QueuePresentInfo& info) const;

private:
    Share<LogicDevice> m_pLogicDevice = nullptr;
    u32 m_queueFamily = 0;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_QUEUE_H