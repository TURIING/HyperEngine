//
// Created by turiing on 2026/1/14.
//

#include "CommandPool.h"
#include "../Graphics.h"
#include "../device/PhysicalDevice.h"
#include "../device/LogicDevice.h"

USING_ENGINE_NAMESPACE_BEGIN
CommandPool::CommandPool() {
    const auto logicDevice = Graphics::Get()->GetLogicDevice();
    const auto physicalDevice = Graphics::Get()->GetPhysicalDevice();

    VkCommandPoolCreateInfo commandPoolCreateInfo = {};
    commandPoolCreateInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    commandPoolCreateInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
    commandPoolCreateInfo.queueFamilyIndex = physicalDevice->GetQueueFamily(QueueType::Graphics);
    CALL_VK(vkCreateCommandPool(logicDevice->GetHandle(), &commandPoolCreateInfo, nullptr, &m_pHandle));
    LOG_DEBUG("CommandPool Created");
}

CommandPool::~CommandPool() {
    const auto logicDevice = Graphics::Get()->GetLogicDevice();
    vkDestroyCommandPool(logicDevice->GetHandle(), m_pHandle, nullptr);
}

USING_ENGINE_NAMESPACE_END
