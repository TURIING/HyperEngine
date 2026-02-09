//
// Created by turiing on 2026/1/14.
//

#ifndef HYPERENGINE_COMMANDBUFFER_H
#define HYPERENGINE_COMMANDBUFFER_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN
class CommandPool;
class LogicDevice;
class CmdManager;
class Buffer;
class Image2D;
class Image;
class Pipeline;

struct BeginRenderPassInfo {
    VkRenderPass renderPass;
    VkFramebuffer framebuffer;
    const std::vector<VkClearValue>* clearValues;
    VkRect2D renderArea;
};

class CommandBuffer final: public VulObject<VkCommandBuffer> {
    friend class CmdManager;
public:
    ~CommandBuffer() override;

    void Reset() const;
    void Begin(bool single = false) const;
    void BeginRenderPass(const BeginRenderPassInfo &beginInfo);
    void BindPipeline(const Pipeline *pipeline) const;
    void SetViewport(const Viewport &viewport) const;
    void SetScissor(const Scissor &scissor);
    void CopyBufferToImage(const Share<Buffer> &pBuffer, const Share<Image> &pImage, const Area2D &area) const;
    void CopyBufferToImage(const void *pData, uint64_t size, const Share<Image> &pImage, const Area2D &area) const;
    void CopyBuffer(Buffer *pSrcBuffer, Buffer *pDstBuffer, uint64_t size, uint64_t srcOffset = 0, uint64_t dstOffset = 0) const;
    void ClearColorImage(const Share<Image> &pImage, const VkClearColorValue &color);
    void TransitionImageLayout(const Share<Image> &pImage, VkImageLayout newLayout, VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT, u32 baseMipLevel = 0, u32 levelCount = 1) const;
    void BindVertexBuffer(Buffer *pVertexBuffer, Buffer *pInstanceBuffer = nullptr) const;
    void BindIndexBuffer(Buffer *pIndexBuffer) const;
    void Draw(u32 vertexCount, u32 instanceCount = 1) const;
    void DrawIndex(u32 indexCount, u32 instanceCount = 1) const;
    void NextSubpass() const;
    void EndRenderPass() const;
    void End() const;

protected:
    explicit CommandBuffer(const Share<CommandPool> &pCmdPool);

private:
    Share<CommandPool> m_pCommandPool = nullptr;
    Share<LogicDevice> m_pLogicalDevice = nullptr;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_COMMANDBUFFER_H