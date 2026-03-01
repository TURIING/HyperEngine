//
// Created by turiing on 2026/1/13.
//

#ifndef HYPERENGINE_DESCRIPTOR_H
#define HYPERENGINE_DESCRIPTOR_H

#include "../../../common/Common.h"

USING_ENGINE_NAMESPACE_BEGIN

class OffsetSize {
public:
    OffsetSize(uint32_t offset, uint32_t size): m_offset(offset), m_size(size) {}
    NODISCARD uint32_t GetOffset() const { return m_offset; }
    NODISCARD uint32_t GetSize() const { return m_size; }

    bool operator==(const OffsetSize &rhs) const {
        return m_offset == rhs.m_offset && m_size == rhs.m_size;
    }

    bool operator!=(const OffsetSize &rhs) const {
        return !operator==(rhs);
    }

private:
    uint32_t m_offset;
    uint32_t m_size;
};

class WriteDescriptorSet {
public:
    WriteDescriptorSet(const VkWriteDescriptorSet &writeDescriptorSet, const VkDescriptorImageInfo &imageInfo)
        :m_writeDescriptorSet(writeDescriptorSet), m_imageInfo(std::make_unique<VkDescriptorImageInfo>(imageInfo)) {
        m_writeDescriptorSet.pImageInfo = m_imageInfo.get();
    }

    WriteDescriptorSet(const VkWriteDescriptorSet &writeDescriptorSet, const VkDescriptorBufferInfo &bufferInfo)
        :m_writeDescriptorSet(writeDescriptorSet), m_bufferInfo(std::make_unique<VkDescriptorBufferInfo>(bufferInfo)) {
        m_writeDescriptorSet.pBufferInfo = m_bufferInfo.get();
    }

	NODISCARD const VkWriteDescriptorSet &GetWriteDescriptorSet() const { return m_writeDescriptorSet; }

private:
    VkWriteDescriptorSet m_writeDescriptorSet{};
    Unique<VkDescriptorImageInfo> m_imageInfo{};
    Unique<VkDescriptorBufferInfo> m_bufferInfo{};
};

class Descriptor {
public:
    virtual ~Descriptor() = default;
    NODISCARD virtual WriteDescriptorSet GetWriteDescriptorSet(uint32_t binding, VkDescriptorType descriptorType) const  = 0;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_DESCRIPTOR_H