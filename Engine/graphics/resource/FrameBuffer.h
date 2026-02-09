//
// Created by turiing on 2026/2/7.
//

#ifndef HYPERENGINE_FRAMEBUFFER_H
#define HYPERENGINE_FRAMEBUFFER_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN

struct FrameBufferCreateInfo {
    VkRenderPass renderPass;
    std::vector<VkImageView> attachments;
    Size2DUI size;
    uint32_t layerCount = 1;
};

class FrameBuffer: public VulObject<VkFramebuffer> {
public:
    explicit FrameBuffer(const FrameBufferCreateInfo &createInfo);
    ~FrameBuffer() override;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_FRAMEBUFFER_H
