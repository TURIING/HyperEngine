//
// Created by turiing on 2026/1/28.
//

#include "Queue.h"
#include "../device/LogicDevice.h"
#include "../surface/SwapChain.h"
#include "../Graphics.h"
#include "../sync/Semaphore.h"
#include "../sync/Fence.h"
#include "../command/CommandBuffer.h"

USING_ENGINE_NAMESPACE_BEGIN
Queue::Queue(u32 queueFamily): m_queueFamily(queueFamily) {
    m_pLogicDevice = Graphics::Get()->GetLogicDevice();
    vkGetDeviceQueue(m_pLogicDevice->GetHandle(), m_queueFamily, 0, &m_pHandle);
    LOG_DEBUG("Queue created");
}

void Queue::Submit(const QueueSubmitInfo &vulSubmitInfo) const {
    std::vector<VkPipelineStageFlags> waitStages;
    waitStages.reserve(vulSubmitInfo.waitSemaphoreCount);
    for (auto i  = 0; i < vulSubmitInfo.waitSemaphoreCount; i++) {
        waitStages.push_back(vulSubmitInfo.waitDstStageMask);
    }

    std::vector<VkSemaphore> waitSemaphores;
    waitSemaphores.reserve(vulSubmitInfo.waitSemaphoreCount);
    for(auto i = 0; i < vulSubmitInfo.waitSemaphoreCount; i++) {
        waitSemaphores.push_back(vulSubmitInfo.pWaitSemaphores[i]->GetHandle());
    }

    std::vector<VkSemaphore> signalSemaphores;
    signalSemaphores.reserve(vulSubmitInfo.signalSemaphoreCount);
    for(auto i = 0; i < vulSubmitInfo.signalSemaphoreCount; i++) {
        signalSemaphores.push_back(vulSubmitInfo.pSignalSemaphores[i]->GetHandle());
    }

    std::vector<VkCommandBuffer> commandBuffers;
    commandBuffers.reserve(vulSubmitInfo.cmdCount);
    for(auto i = 0; i < vulSubmitInfo.cmdCount; i++) {
        commandBuffers.push_back(vulSubmitInfo.pCmd[i]->GetHandle());
    }

    const VkSubmitInfo submitInfo = {
        .sType = VK_STRUCTURE_TYPE_SUBMIT_INFO,
        .waitSemaphoreCount = vulSubmitInfo.waitSemaphoreCount,
        .pWaitSemaphores = waitSemaphores.data(),
        .pWaitDstStageMask = waitStages.data(),
        .commandBufferCount = vulSubmitInfo.cmdCount,
        .pCommandBuffers = commandBuffers.data(),
        .signalSemaphoreCount = vulSubmitInfo.signalSemaphoreCount,
        .pSignalSemaphores = signalSemaphores.data(),
    };
    CALL_VK(vkQueueSubmit(m_pHandle, 1, &submitInfo, vulSubmitInfo.pFence ? vulSubmitInfo.pFence->GetHandle() : nullptr));
}

void Queue::Wait() const {
    CALL_VK(vkQueueWaitIdle(m_pHandle));
}

void Queue::Present(const QueuePresentInfo &info) const {
    std::vector<VkSwapchainKHR> swapChains;
    swapChains.reserve(info.swapChainCount);
    for (auto i = 0; i < info.swapChainCount; i++) {
        swapChains.push_back(info.pSwapChain[i]->GetHandle());
    }

    std::vector<VkSemaphore> semaphores;
    semaphores.reserve(info.waitSemaphoreCount);
    for (auto i = 0; i < info.waitSemaphoreCount; i++) {
        semaphores.push_back(info.pWaitSemaphores[i]->GetHandle());
    }

    const VkPresentInfoKHR presentInfo = {
        .sType				= VK_STRUCTURE_TYPE_PRESENT_INFO_KHR,
        .waitSemaphoreCount = TO_U32(semaphores.size()),
        .pWaitSemaphores	= semaphores.data(),
        .swapchainCount		= TO_U32(swapChains.size()),
        .pSwapchains		= swapChains.data(),
        .pImageIndices		= info.imageIndex,
    };
    vkQueuePresentKHR(m_pHandle, &presentInfo);
}

USING_ENGINE_NAMESPACE_END
