//
// Created by turiing on 2026/1/15.
//

#ifndef HYPERENGINE_RENDERSTAGE_H
#define HYPERENGINE_RENDERSTAGE_H

#include "../../common/Common.h"
#include "../resource/FrameBuffer.h"
#include "../pipeline/RenderPass.h"

USING_ENGINE_NAMESPACE_BEGIN
class Descriptor;
class SwapChain;
class ImageDepth;
class Image;

struct RenderArea {
	Size2DUI extent;
	Point2DI offset;
	float aspectRatio = 1.0f;

	bool operator==(const RenderArea &rhs) const {
		return extent == rhs.extent && offset == rhs.offset;
	}

	bool operator!=(const RenderArea &rhs) const {
		return !operator==(rhs);
	}
};

class RenderStage {
public:
	explicit RenderStage(
	    std::vector<Attachment> attachments = {},
	    std::vector<SubpassInfo> subpasses = {},
	    std::optional<Viewport> viewport = std::nullopt,
	    std::optional<Scissor> scissor = std::nullopt);

	void Update();

	NODISCARD const std::vector<Attachment> &GetAttachments() const { return m_vecAttachments; }
	NODISCARD const std::vector<SubpassInfo> &GetSubpasses() const { return m_vecSubpasses; }
    NODISCARD FrameBuffer* GetFrameBuffer() const;
    NODISCARD const Viewport& GetViewport() const { return m_viewport; }
    NODISCARD const Scissor& GetScissor() const { return m_scissor; }
	NODISCARD const RenderPass *GetRenderPass() const { return m_pRenderPass.get(); }
    NODISCARD const std::vector<VkClearValue>& GetClearValues() const { return m_vecClearValues; }

private:
	std::vector<Attachment> m_vecAttachments;
	std::vector<SubpassInfo> m_vecSubpasses;
    std::vector<Share<Image>> m_vecAttachmentImages;
    std::vector<VkClearValue> m_vecClearValues;
    std::vector<Unique<FrameBuffer>> m_vecFramebuffer;
	Viewport m_viewport;
    Scissor m_scissor;
	Unique<RenderPass> m_pRenderPass;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_RENDERSTAGE_H