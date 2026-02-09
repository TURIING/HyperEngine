//
// Created by turiing on 2026/2/3.
//

#include "Fence.h"
#include "../../graphics/Graphics.h"
#include "../device/LogicDevice.h"

USING_ENGINE_NAMESPACE_BEGIN

Fence::Fence(bool signaled) {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();

    VkFenceCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
    createInfo.flags = signaled ? VK_FENCE_CREATE_SIGNALED_BIT : 0;

    CALL_VK(vkCreateFence(logicalDevice->GetHandle(), &createInfo, nullptr, &m_pHandle));
    LOG_DEBUG("Fence created");
}

Fence::~Fence() {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();
    vkDestroyFence(logicalDevice->GetHandle(), m_pHandle, nullptr);
}

WaitState Fence::Wait(uint64_t timeout) const {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();
    auto result = vkWaitForFences(logicalDevice->GetHandle(), 1, &m_pHandle, true, timeout);

    if (result == VK_SUCCESS) {
        return WaitState::Success;
    } else if (result == VK_TIMEOUT) {
        return WaitState::Timeout;
    }
    return WaitState::Failed;
}

void Fence::Reset() const {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();
    vkResetFences(logicalDevice->GetHandle(), 1, &m_pHandle);
}

USING_ENGINE_NAMESPACE_END
