//
// Created by turiing on 2026/2/7.
//

#include "FrameBuffer.h"
#include "../../graphics/Graphics.h"
#include "../device/LogicDevice.h"

USING_ENGINE_NAMESPACE_BEGIN

FrameBuffer::FrameBuffer(const FrameBufferCreateInfo &createInfo) {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();

    VkFramebufferCreateInfo framebufferInfo = {};
    framebufferInfo.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    framebufferInfo.renderPass = createInfo.renderPass;
    framebufferInfo.attachmentCount = static_cast<uint32_t>(createInfo.attachments.size());
    framebufferInfo.pAttachments = createInfo.attachments.data();
    framebufferInfo.width = createInfo.size.width;
    framebufferInfo.height = createInfo.size.height;
    framebufferInfo.layers = createInfo.layerCount;

    CALL_VK(vkCreateFramebuffer(logicalDevice->GetHandle(), &framebufferInfo, nullptr, &m_pHandle));
}

FrameBuffer::~FrameBuffer() {
    auto logicalDevice = Graphics::Get()->GetLogicDevice();
    vkDestroyFramebuffer(logicalDevice->GetHandle(), m_pHandle, nullptr);
}

USING_ENGINE_NAMESPACE_END
