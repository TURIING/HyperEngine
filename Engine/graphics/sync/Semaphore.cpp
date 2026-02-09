//
// Created by turiing on 2026/2/3.
//

#include "Semaphore.h"
#include "../../graphics/Graphics.h"
#include "../device/LogicDevice.h"

USING_ENGINE_NAMESPACE_BEGIN

Semaphore::Semaphore() {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();

    VkSemaphoreCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

    CALL_VK(vkCreateSemaphore(logicalDevice->GetHandle(), &createInfo, nullptr, &m_pHandle));
    LOG_DEBUG("Semaphore created");
}

Semaphore::~Semaphore() {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();
    vkDestroySemaphore(logicalDevice->GetHandle(), m_pHandle, nullptr);
}

USING_ENGINE_NAMESPACE_END
