//
// Created by turiing on 2026/2/1.
//

#include "DescriptorPool.h"
#include "../../Graphics.h"
#include "../../device/LogicDevice.h"

constexpr uint32_t DESCRIPTOR_POOL_MAX_SET_COUNT = 100;

USING_ENGINE_NAMESPACE_BEGIN
DescriptorPool::DescriptorPool(const std::vector<DescriptorPoolSizeInfo> &infos) {
    auto pLogicDevice = Graphics::Get()->GetLogicDevice();
    std::vector<VkDescriptorPoolSize> poolSizes(infos.size());

    for(auto i = 0; i < infos.size(); ++i) {
        poolSizes[i] = {
            .type = infos[i].type,
            .descriptorCount = infos[i].count,
        };
    }

    const VkDescriptorPoolCreateInfo poolCreateInfo {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .maxSets = DESCRIPTOR_POOL_MAX_SET_COUNT,
        .poolSizeCount = static_cast<uint32_t>(poolSizes.size()),
        .pPoolSizes = poolSizes.data(),
    };
    CALL_VK(vkCreateDescriptorPool(pLogicDevice->GetHandle(), &poolCreateInfo, nullptr, &m_pHandle));
    LOG_DEBUG("Created descriptor pool");
}

DescriptorPool::~DescriptorPool() {
    auto pLogicDevice = Graphics::Get()->GetLogicDevice();
    vkDestroyDescriptorPool(pLogicDevice->GetHandle(), m_pHandle, nullptr);
}

USING_ENGINE_NAMESPACE_END
