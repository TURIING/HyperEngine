//
// Created by turiing on 2026/2/2.
//

#include "PipelineLayout.h"
#include "../../graphics/Graphics.h"
#include "../device/LogicDevice.h"
#include "../resource/descriptor/DescriptorSetLayout.h"

USING_ENGINE_NAMESPACE_BEGIN

PipelineLayout::PipelineLayout(Share<DescriptorSetLayout> descriptorSetLayout, const std::vector<VkPushConstantRange> &pushConstantRanges) {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();

    VkPipelineLayoutCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    createInfo.setLayoutCount = 1;
    createInfo.pSetLayouts = static_cast<VkDescriptorSetLayout[]>(descriptorSetLayout->GetHandle());
    createInfo.pushConstantRangeCount = static_cast<uint32_t>(pushConstantRanges.size());
    createInfo.pPushConstantRanges = pushConstantRanges.data();

    CALL_VK(vkCreatePipelineLayout(logicalDevice->GetHandle(), &createInfo, nullptr, &m_pHandle));
}

PipelineLayout::~PipelineLayout() {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();
    vkDestroyPipelineLayout(logicalDevice->GetHandle(), m_pHandle, nullptr);
}

USING_ENGINE_NAMESPACE_END
