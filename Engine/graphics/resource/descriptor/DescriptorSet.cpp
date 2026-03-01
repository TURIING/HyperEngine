//
// Created by turiing on 2026/1/16.
//

#include "DescriptorSet.h"
#include "../../pipeline/Pipeline.h"
#include "../../Graphics.h"
#include "../../device/LogicDevice.h"
#include "../../pipeline/PipelineLayout.h"
#include "DescriptorPool.h"
#include "DescriptorSetLayout.h"

USING_ENGINE_NAMESPACE_BEGIN
    DescriptorSet::DescriptorSet(Pipeline *pPipeline)
    :m_pipelineLayout(pPipeline->GetPipelineLayout()->GetHandle()),
     m_pipelineBindPoint(pPipeline->GetBindPoint()),
     m_descriptorPool(Graphics::Get()->GetDescriptorPool()->GetHandle()) {

    auto logicalDevice = Graphics::Get()->GetLogicDevice();
    const VkDescriptorSetLayout layouts[1] = { pPipeline->GetDescriptorSetLayout()->GetHandle() };

    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo = {};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.descriptorPool = m_descriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = 1;
    descriptorSetAllocateInfo.pSetLayouts = layouts;
    CALL_VK(vkAllocateDescriptorSets(logicalDevice->GetHandle(), &descriptorSetAllocateInfo, &m_pHandle));
}

DescriptorSet::~DescriptorSet() {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();
	CALL_VK(vkFreeDescriptorSets(logicalDevice->GetHandle(), m_descriptorPool, 1, &m_pHandle));
}

void DescriptorSet::BindDescriptor(CommandBuffer *pCommandBuffer) const {
	vkCmdBindDescriptorSets(pCommandBuffer->GetHandle(), m_pipelineBindPoint, m_pipelineLayout, 0, 1, &m_pHandle, 0, nullptr);
}

void DescriptorSet::Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites) {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();
    vkUpdateDescriptorSets(logicalDevice->GetHandle(), descriptorWrites.size(), descriptorWrites.data(), 0, nullptr);
}

USING_ENGINE_NAMESPACE_END
