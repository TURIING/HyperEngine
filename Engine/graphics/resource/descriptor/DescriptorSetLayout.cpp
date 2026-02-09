//
// Created by turiing on 2026/2/2.
//

#include "DescriptorSetLayout.h"
#include "../../../graphics/Graphics.h"
#include "../../device/LogicDevice.h"

USING_ENGINE_NAMESPACE_BEGIN

DescriptorSetLayout::DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding> &bindings) {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();

    VkDescriptorSetLayoutCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    createInfo.bindingCount = static_cast<uint32_t>(bindings.size());
    createInfo.pBindings = bindings.data();

    CALL_VK(vkCreateDescriptorSetLayout(logicalDevice->GetHandle(), &createInfo, nullptr, &m_pHandle));
}

DescriptorSetLayout::~DescriptorSetLayout() {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();
    vkDestroyDescriptorSetLayout(logicalDevice->GetHandle(), m_pHandle, nullptr);
}

USING_ENGINE_NAMESPACE_END
