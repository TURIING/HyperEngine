//
// Created by turiing on 2026/1/16.
//

#ifndef HYPERENGINE_DESCRIPTORSET_H
#define HYPERENGINE_DESCRIPTORSET_H

#include "../../../common/Common.h"
#include "../../VulObject.h"
USING_ENGINE_NAMESPACE_BEGIN
class Pipeline;
class CommandBuffer;

class DescriptorSet: public VulObject<VkDescriptorSet> {
public:
    explicit DescriptorSet(Pipeline *pPipeline);
    ~DescriptorSet() override;
    void BindDescriptor(CommandBuffer *pCommandBuffer) const;
    static void Update(const std::vector<VkWriteDescriptorSet> &descriptorWrites);

private:
    VkPipelineLayout m_pipelineLayout;
    VkPipelineBindPoint m_pipelineBindPoint;
    VkDescriptorPool m_descriptorPool = VK_NULL_HANDLE;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_DESCRIPTORSET_H