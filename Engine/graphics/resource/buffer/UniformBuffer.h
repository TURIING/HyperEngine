//
// Created by turiing on 2026/1/13.
//

#ifndef HYPERENGINE_UNIFORMBUFFER_H
#define HYPERENGINE_UNIFORMBUFFER_H

#include "../../../common/Common.h"
#include "../descriptor/Descriptor.h"
#include "Buffer.h"

USING_ENGINE_NAMESPACE_BEGIN

class UniformBuffer: public Descriptor, public Buffer {
public:
    explicit UniformBuffer(VkDeviceSize size, const void *pData = nullptr);
    void Update(const void *pNewData) const;
    NODISCARD WriteDescriptorSet GetWriteDescriptorSet(uint32_t binding, VkDescriptorType descriptorType) const override;
    static VkDescriptorSetLayoutBinding GetDescriptorSetLayout(uint32_t binding, VkDescriptorType descriptorType, VkShaderStageFlags stage, uint32_t count);
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_UNIFORMBUFFER_H