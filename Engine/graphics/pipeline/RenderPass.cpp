//
// Created by turiing on 2026/2/5.
//

#include "RenderPass.h"
#include "../Graphics.h"
#include "../device/LogicDevice.h"

USING_ENGINE_NAMESPACE_BEGIN

class SubpassDescription: NoCopyable {
public:
    SubpassDescription(std::vector<VkAttachmentReference> vecColorAttachmentRef, std::optional<VkAttachmentReference> depthAttachment)
        : m_vecColorAttachmentRef(std::move(vecColorAttachmentRef)), m_depthAttachment(depthAttachment) {
        m_subpassDescription.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
        m_subpassDescription.colorAttachmentCount = TO_U32(m_vecColorAttachmentRef.size());
        m_subpassDescription.pColorAttachments = m_vecColorAttachmentRef.data();
        if (m_depthAttachment) {
            m_subpassDescription.pDepthStencilAttachment = &*m_depthAttachment;
        }
    }

    NODISCARD VkSubpassDescription GetDescription() const { return m_subpassDescription; }

private:
    std::vector<VkAttachmentReference> m_vecColorAttachmentRef;
    std::optional<VkAttachmentReference> m_depthAttachment;
    VkSubpassDescription m_subpassDescription {};
};

RenderPass::RenderPass(const std::vector<Attachment> &info, const std::vector<SubpassInfo> &vecSubpassInfo) {
    auto pLogicDevice = Graphics::Get()->GetLogicDevice();

    auto attachments = createAttachmentDescriptions(info);
    auto [vecSubpassDescription, vecSubpassDependency] = createSubpassDescriptionAndDependency(info, vecSubpassInfo);

    std::vector<VkSubpassDescription> vecRealSubpassDescription;
    for (const auto &des: vecSubpassDescription) {
        vecRealSubpassDescription.emplace_back(des->GetDescription());
    }

    const VkRenderPassCreateInfo renderPassInfo = {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
        .attachmentCount = static_cast<uint32_t>(attachments.size()),
        .pAttachments = attachments.data(),
        .subpassCount = TO_U32(vecRealSubpassDescription.size()),
        .pSubpasses = vecRealSubpassDescription.data(),
        .dependencyCount = TO_U32(vecSubpassDependency.size()),
        .pDependencies = vecSubpassDependency.data()
    };
    CALL_VK(vkCreateRenderPass(pLogicDevice->GetHandle(), &renderPassInfo, nullptr, &m_pHandle));
    LOG_DEBUG("RenderPass created");
}

RenderPass::~RenderPass() {
    auto pLogicDevice = Graphics::Get()->GetLogicDevice();
    vkDestroyRenderPass(pLogicDevice->GetHandle(), m_pHandle, nullptr);
}

std::vector<VkAttachmentDescription> RenderPass::createAttachmentDescriptions(const std::vector<Attachment> &info) {
    std::vector<VkAttachmentDescription> attachments;
    for(const auto &attachment : info) {
        // 颜色附着
        if(attachment.type == AttachmentType::Image) {
            attachments.emplace_back(VkAttachmentDescription {
                .format = gPixelFormatToVkFormat[TO_U32(attachment.format)],
                .samples = static_cast<VkSampleCountFlagBits>(attachment.samples),

                // 对颜色和深度缓冲起效
                .loadOp = static_cast<VkAttachmentLoadOp>(attachment.loadOp),
                .storeOp = static_cast<VkAttachmentStoreOp>(attachment.storeOp),

                // 对模板缓冲起效
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,

                .initialLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
                .finalLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL
            });
        }
        // 深度附着
        else if(attachment.type == AttachmentType::Depth) {
            attachments.emplace_back(VkAttachmentDescription {
                .format = gPixelFormatToVkFormat[TO_U32(attachment.format)],
                .samples = static_cast<VkSampleCountFlagBits>(attachment.samples),
                .loadOp = static_cast<VkAttachmentLoadOp>(attachment.loadOp),
                .storeOp = static_cast<VkAttachmentStoreOp>(attachment.storeOp),
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
                .finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL
            });
        }
        else if (attachment.type == AttachmentType::SwapChain) {
            attachments.emplace_back(VkAttachmentDescription {
                .format = gPixelFormatToVkFormat[TO_U32(attachment.format)],
                .samples = static_cast<VkSampleCountFlagBits>(attachment.samples),
                .loadOp = static_cast<VkAttachmentLoadOp>(attachment.loadOp),
                .storeOp = static_cast<VkAttachmentStoreOp>(attachment.storeOp),
                .stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
                .stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
                .initialLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
                .finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR
            });
        }
    }
    return attachments;
}

std::pair<std::vector<Unique<SubpassDescription>>, std::vector<VkSubpassDependency>>
RenderPass::createSubpassDescriptionAndDependency(const std::vector<Attachment> &info, const std::vector<SubpassInfo> &vecSubpassInfo) {
    auto funcFindAttachment = [](const std::vector<Attachment> &vecAttachment, u32 binding) -> std::optional<Attachment> {
        auto it = std::find_if(vecAttachment.begin(), vecAttachment.end(), [&](const Attachment &attachment) {
            return attachment.binding == binding;
        });
        if (it != vecAttachment.end())
            return *it;
        return std::nullopt;
    };

    std::vector<Unique<SubpassDescription>> vecSubpassDescription;
    std::vector<VkSubpassDependency> vecSubpassDependency;

    for (const auto &subpassInfo: vecSubpassInfo) {
        std::vector<VkAttachmentReference> vecColorAttachmentRef;
        std::optional<VkAttachmentReference> depthAttachmentRef;
        for (auto binding: subpassInfo.attachmentBindings) {
            auto att = funcFindAttachment(info, binding);
            if (!att) {
                LOG_WARNING("Failed to find attachment {}", binding);
                continue;
            }

            if (att->type == AttachmentType::Depth) {
                depthAttachmentRef->attachment = att->binding;
                depthAttachmentRef->layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
            }
            else {
                VkAttachmentReference attachmentRef;
                attachmentRef.attachment = att->binding;
                attachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
                vecColorAttachmentRef.push_back(attachmentRef);
            }
        }
        vecSubpassDescription.emplace_back(std::make_unique<SubpassDescription>(vecColorAttachmentRef, depthAttachmentRef));

        VkSubpassDependency subpassDependency = {};
        subpassDependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        subpassDependency.dstStageMask = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        subpassDependency.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
        subpassDependency.dependencyFlags = VK_DEPENDENCY_BY_REGION_BIT;

        if (subpassInfo.binding == vecSubpassInfo.size()) {
            subpassDependency.dstSubpass = VK_SUBPASS_EXTERNAL;
            subpassDependency.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            subpassDependency.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
            subpassDependency.dstAccessMask = VK_ACCESS_MEMORY_READ_BIT;
        } else {
            subpassDependency.dstSubpass = subpassInfo.binding;
        }

        if (subpassInfo.binding == 0) {
            subpassDependency.srcSubpass = VK_SUBPASS_EXTERNAL;
            subpassDependency.srcStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT;
            subpassDependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
            subpassDependency.srcAccessMask = VK_ACCESS_MEMORY_READ_BIT;
            subpassDependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT | VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        } else {
            subpassDependency.srcSubpass = subpassInfo.binding - 1;
        }
        vecSubpassDependency.push_back(subpassDependency);
    }

    return {std::move(vecSubpassDescription), std::move(vecSubpassDependency)};
}

USING_ENGINE_NAMESPACE_END
