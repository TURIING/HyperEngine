//
// Created by turiing on 2026/1/13.
//

#include "UniformBuffer.h"

USING_ENGINE_NAMESPACE_BEGIN
UniformBuffer::UniformBuffer(VkDeviceSize size, const void *pData)
    :Buffer(size, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT){
}

void UniformBuffer::Update(const void *pNewData) const {
    WriteData(0, m_size, pNewData);
}

WriteDescriptorSet UniformBuffer::GetWriteDescriptorSet(uint32_t binding, VkDescriptorType descriptorType) const {
    VkDescriptorBufferInfo bufferInfo = {};
    bufferInfo.buffer = m_pHandle;
    bufferInfo.offset = 0;
    bufferInfo.range = m_size;

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = VK_NULL_HANDLE; // Will be set in the descriptor handler.
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.descriptorType = descriptorType;
    return {descriptorWrite, bufferInfo};
}

VkDescriptorSetLayoutBinding UniformBuffer::GetDescriptorSetLayout(uint32_t binding,
    VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count) {
    VkDescriptorSetLayoutBinding descriptorSetLayoutBinding = {};
    descriptorSetLayoutBinding.binding = binding;
    descriptorSetLayoutBinding.descriptorType = descriptorType;
    descriptorSetLayoutBinding.descriptorCount = 1;
    descriptorSetLayoutBinding.stageFlags = stage;
    descriptorSetLayoutBinding.pImmutableSamplers = nullptr;
    return descriptorSetLayoutBinding;
}

USING_ENGINE_NAMESPACE_END
