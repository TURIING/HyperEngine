//
// Created by turiing on 2026/2/2.
//

#ifndef HYPERENGINE_PIPELINELAYOUT_H
#define HYPERENGINE_PIPELINELAYOUT_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN

class DescriptorSetLayout;

class PipelineLayout: public VulObject<VkPipelineLayout> {
public:
    explicit PipelineLayout(DescriptorSetLayout* descriptorSetLayout, const std::vector<VkPushConstantRange> &pushConstantRanges = {});
    ~PipelineLayout() override;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_PIPELINELAYOUT_H
