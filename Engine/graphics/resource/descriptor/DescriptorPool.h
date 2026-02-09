//
// Created by turiing on 2026/2/1.
//

#ifndef HYPERENGINE_DESCRIPTORPOOL_H
#define HYPERENGINE_DESCRIPTORPOOL_H
#include "../../../common/Common.h"
#include "../../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN
struct DescriptorPoolSizeInfo {
    VkDescriptorType type;
    uint32_t count;
};

class DescriptorPool: public VulObject<VkDescriptorPool> {
public:
    explicit DescriptorPool(const std::vector<DescriptorPoolSizeInfo> &infos = {
        {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 100},
        {VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 100},
        {VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 50},
        {VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 50},
    });
    ~DescriptorPool() override;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_DESCRIPTORPOOL_H