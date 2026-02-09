//
// Created by turiing on 2026/2/5.
//

#ifndef HYPERENGINE_RENDERPASS_H
#define HYPERENGINE_RENDERPASS_H
#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN
class SubpassDescription;

class RenderPass : public VulObject<VkRenderPass> {
public:
    explicit RenderPass(const std::vector<Attachment> &info, const std::vector<SubpassInfo> &vecSubpassInfo);
    ~RenderPass() override;

private:
    NODISCARD static std::vector<VkAttachmentDescription> createAttachmentDescriptions(const std::vector<Attachment> &info);
    NODISCARD static std::pair<std::vector<Unique<SubpassDescription>>, std::vector<VkSubpassDependency>>
    createSubpassDescriptionAndDependency(const std::vector<Attachment> &info, const std::vector<SubpassInfo> &vecSubpassInfo);
};


USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_RENDERPASS_H