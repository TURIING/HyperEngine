//
// Created by turiing on 2026/2/2.
//

#ifndef HYPERENGINE_DESCRIPTORSETLAYOUT_H
#define HYPERENGINE_DESCRIPTORSETLAYOUT_H

#include "../../../common/Common.h"
#include "../../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN

class DescriptorSetLayout: public VulObject<VkDescriptorSetLayout> {
public:
    explicit DescriptorSetLayout(const std::vector<VkDescriptorSetLayoutBinding> &bindings);
    ~DescriptorSetLayout() override;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_DESCRIPTORSETLAYOUT_H
