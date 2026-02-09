//
// Created by turiing on 2026/1/15.
//

#include "RenderStage.h"
#include "RenderPass.h"
#include "../resource/image/Image2D.h"
#include "../resource/image/ImageDepth.h"
#include "../../device/Windows.h"
#include "../../device/Window.h"
#include "../surface/PresentManager.h"
#include "../surface/SwapChain.h"
#include "../Graphics.h"
#include "../resource/FrameBuffer.h"

USING_ENGINE_NAMESPACE_BEGIN
NODISCARD VkClearValue gTransClearValueToVkClearValue(AttachmentType type, const ClearValue &value) {
    switch (type) {
        case AttachmentType::Image:
        case AttachmentType::SwapChain: {
            auto [r, g, b, a] = value.color;
            return {
                .color = { r, g, b, a}
            };
        }
        case AttachmentType::Depth: {
            auto [depth, stencil] = value.depthStencil;
            return {
                .depthStencil = { depth, stencil }
            };
        }
        default: assert(false);
    }
}

RenderStage::RenderStage(std::vector<Attachment> attachments, std::vector<SubpassInfo> subpasses, std::optional<Viewport> viewport, std::optional<Scissor> scissor)
    :m_vecAttachments(std::move(attachments)), m_vecSubpasses(std::move(subpasses)) {
    m_viewport = viewport ? *viewport : Windows::Get()->GetWindow()->GetSize();
    m_scissor = scissor ? *scissor : m_viewport;
    m_pRenderPass = std::make_unique<RenderPass>(m_vecAttachments, m_vecSubpasses);
    auto pPresentManager = Graphics::Get()->GetPresentManager();

    auto funcHasSwapChain = [&]() -> std::optional<u32> {
        auto it = std::find_if(attachments.begin(), attachments.end(), [&](const Attachment &attachment) {
            return attachment.type == AttachmentType::SwapChain;
        });
        if (it != attachments.end()) {
            return pPresentManager->GetSwapChain(it->name)->GetImageCount();
        }
        return std::nullopt;
    };

    auto count = funcHasSwapChain();
    for (auto i = 0; i < (count ? *count : 1); i++) {
        std::vector<VkImageView> vecImageViews;
        for (auto &attachment: m_vecAttachments) {
            switch(attachment.type) {
                case AttachmentType::Image: {
                    Image2DCreateInfo info;
                    info.size = m_viewport.size;
                    info.format = attachment.format;
                    m_vecAttachmentImages.emplace_back(Image2D::Create(info));
                    vecImageViews.push_back(m_vecAttachmentImages.back()->GetImageView());
                    break;
                }
                case AttachmentType::Depth: {
                    TODO
                }
                case AttachmentType::SwapChain: {
                    auto pSwapChain = pPresentManager->GetSwapChain(attachment.name);
                    auto pImage = pPresentManager->GetCurrentImage(attachment.name, i);
                    vecImageViews.push_back(pImage->GetImageView());
                    break;
                }
                default: assert(false);
            }
            m_vecClearValues.emplace_back(gTransClearValueToVkClearValue(attachment.type, attachment.clearValue));
        }
        FrameBufferCreateInfo info;
        info.renderPass = m_pRenderPass->GetHandle();
        info.size = m_viewport.size;
        info.attachments = std::move(vecImageViews);
        m_vecFramebuffer.emplace_back(std::make_unique<FrameBuffer>(info));
    }
}

FrameBuffer * RenderStage::GetFrameBuffer() const {
    if (m_vecFramebuffer.size() == 1) {
        return m_vecFramebuffer.begin()->get();
    }

    auto pPresentManager = Graphics::Get()->GetPresentManager();
    return m_vecFramebuffer[pPresentManager->GetImageIndex()].get();
}

USING_ENGINE_NAMESPACE_END
