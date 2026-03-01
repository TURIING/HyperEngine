//
// Created by turiing on 2026/1/14.
//

#include "CommandBuffer.h"

#include "CommandPool.h"
#include "../Graphics.h"
#include "../device/LogicDevice.h"
#include "../resource/image/Image.h"
#include "../resource/buffer/Buffer.h"
#include "../pipeline/Pipeline.h"
#include "../pipeline/PipelineLayout.h"
#include "../resource/descriptor/DescriptorSet.h"

USING_ENGINE_NAMESPACE_BEGIN
struct LayoutTransition {
    VkAccessFlags srcAccessMask;
    VkAccessFlags dstAccessMask;
    VkPipelineStageFlags sourceStage;
    VkPipelineStageFlags destinationStage;
};

using TransitionKey = uint64_t;

inline TransitionKey MakeKey(VkImageLayout oldLayout, VkImageLayout newLayout) {
    return (static_cast<uint64_t>(oldLayout) << 32) | static_cast<u32>(newLayout);
}

static const std::unordered_map<TransitionKey, LayoutTransition> TRANSITION_TABLE = {
    {MakeKey(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_GENERAL),
     {0, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL),
     {VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_GENERAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL),
     {VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL),
     {VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_GENERAL),
     {VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT | VK_ACCESS_SHADER_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COMPUTE_SHADER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL),
     {0, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL),
     {0, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
     {0, VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
     {VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL),
     {0, VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL),
     {VK_ACCESS_SHADER_READ_BIT, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL),
     {VK_ACCESS_SHADER_READ_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL),
     {VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_SHADER_READ_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR),
     {VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_NONE, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_PRESENT_SRC_KHR, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL),
     {VK_ACCESS_NONE, VK_ACCESS_TRANSFER_WRITE_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
     {VK_ACCESS_SHADER_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL, VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL),
     {VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_ACCESS_TRANSFER_READ_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
     {VK_ACCESS_TRANSFER_READ_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
     {VK_ACCESS_TRANSFER_WRITE_BIT, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL),
     {VK_ACCESS_NONE, VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT}},
    {MakeKey(VK_IMAGE_LAYOUT_UNDEFINED, VK_IMAGE_LAYOUT_PRESENT_SRC_KHR),
     {VK_ACCESS_NONE, VK_ACCESS_NONE, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT}},
};

CommandBuffer::CommandBuffer(const Share<CommandPool> &pCmdPool): m_pCommandPool(pCmdPool) {
    m_pLogicalDevice = Graphics::Get()->GetLogicDevice();
    VkCommandBufferAllocateInfo allocInfo = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO,
        .commandPool = m_pCommandPool->GetHandle(),
        .level = VK_COMMAND_BUFFER_LEVEL_PRIMARY,                                                   // 可以被提交到队列进行执行，但不能被其它指令缓冲对象调用
        .commandBufferCount = 1,
    };
    CALL_VK(vkAllocateCommandBuffers(m_pLogicalDevice->GetHandle(), &allocInfo, &m_pHandle));
    LOG_DEBUG("Command buffer created");
}

CommandBuffer::~CommandBuffer() {
    vkFreeCommandBuffers(m_pLogicalDevice->GetHandle(), m_pCommandPool->GetHandle(), 1, &m_pHandle);
}

void CommandBuffer::Reset() const {
    vkResetCommandBuffer(m_pHandle, 0);
}

void CommandBuffer::Begin(bool single) const {
    VkCommandBufferBeginInfo beginInfo = {};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = single ? VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT : 0;
    CALL_VK(vkBeginCommandBuffer(m_pHandle, &beginInfo));
}

void CommandBuffer::BeginRenderPass(const BeginRenderPassInfo &beginInfo) {
    VkRenderPassBeginInfo info {
        .sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO,
        .renderPass = beginInfo.renderPass,
        .framebuffer = beginInfo.framebuffer,
        .renderArea = beginInfo.renderArea,
        .clearValueCount = TO_U32(beginInfo.clearValues->size()),
        .pClearValues = beginInfo.clearValues->data(),
    };
    vkCmdBeginRenderPass(m_pHandle, &info, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::BindDescriptor(const Pipeline *pipeline, const DescriptorSet *pSet) const {
    const auto bindPoint = pipeline->GetBindPoint();
    const auto layout = pipeline->GetPipelineLayout()->GetHandle();
    const auto set = pSet->GetHandle();

    vkCmdBindDescriptorSets(m_pHandle, bindPoint, layout, 0, 1, &set, 0, nullptr);
}

void CommandBuffer::BindPipeline(const Pipeline *pipeline) const {
    vkCmdBindPipeline(m_pHandle, pipeline->GetBindPoint(), pipeline->GetHandle());
}

void CommandBuffer::SetViewport(const Viewport &viewport) const {
    VkViewport vp;
    vp.x = viewport.offset.x;
    vp.y = viewport.offset.y;
    vp.width = viewport.size.width;
    vp.height = viewport.size.height;
    vp.minDepth = 0.0f;
    vp.maxDepth = 1.0f;
    vkCmdSetViewport(m_pHandle, 0, 1, &vp);
}

void CommandBuffer::SetScissor(const Scissor &scissor) {
    VkRect2D sc;
    sc.offset = std::bit_cast<VkOffset2D>(scissor.offset);
    sc.extent = std::bit_cast<VkExtent2D>(scissor.size);
    vkCmdSetScissor(m_pHandle, 0, 1, &sc);
}

void CommandBuffer::CopyBufferToImage(Buffer* pBuffer, Image* pImage, const Area2D &area) const {
    TransitionImageLayout(pImage, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL);

    const VkBufferImageCopy region = {
        .bufferOffset	   = 0,
        .bufferRowLength   = 0,
        .bufferImageHeight = 0,
        .imageSubresource =
        {
            .aspectMask		= pImage->GetAspectFlags(),
            .mipLevel		= 0,
            .baseArrayLayer = 0,
            .layerCount		= 1,
        },
        .imageOffset =
        {
            .x = TO_I32(area.offset.x),
            .y = TO_I32(area.offset.y),
            .z = 0,
        },
        .imageExtent =
        {
            .width	= area.size.width,
            .height = area.size.height,
            .depth	= 1,
        },
    };
    vkCmdCopyBufferToImage(m_pHandle, pBuffer->GetHandle(), pImage->GetHandle(), VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &region);

    if ((pImage->GetUsage() & VK_IMAGE_USAGE_SAMPLED_BIT) != 0) {
        TransitionImageLayout(pImage, VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL);
    }
}

void CommandBuffer::CopyBuffer(Buffer *pSrcBuffer, Buffer *pDstBuffer, uint64_t size, uint64_t srcOffset, uint64_t dstOffset) const {
    VkBufferCopy region;
    region.size = size;
    region.srcOffset = srcOffset;
    region.dstOffset = dstOffset;
    vkCmdCopyBuffer(m_pHandle, pSrcBuffer->GetHandle(), pDstBuffer->GetHandle(), 1, &region);
}

void CommandBuffer::ClearColorImage(const Share<Image> &pImage, const VkClearColorValue &color) {
    const VkImageSubresourceRange range{
        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
        .baseMipLevel = 0,
        .levelCount = 1,
        .baseArrayLayer = 0,
        .layerCount = 1,
    };
    vkCmdClearColorImage(m_pHandle, pImage->GetHandle(), pImage->GetCurrentImageLayout(), &color, 1, &range);

}

/**
 * 转换图像布局
 * @param pImage 图像对象
 * @param newLayout 新布局
 * @param aspectFlags 图像类型
 * @param baseMipLevel 指定起始 mipmap 层级
 * @param levelCount 指定要操作的 mipmap 层数量
 */
void CommandBuffer::TransitionImageLayout(Image* pImage, VkImageLayout newLayout, VkImageAspectFlags aspectFlags, u32 baseMipLevel, u32 levelCount) const {
    const auto currentLayout = pImage->GetCurrentImageLayout();
    if (currentLayout == newLayout) return;

    auto it = TRANSITION_TABLE.find(MakeKey(currentLayout, newLayout));
    if (it == TRANSITION_TABLE.end()) {
        LOG_CRITICAL("TransitionImageLayout: Unsupported layout transition from {} to {}!", TO_U32(currentLayout), TO_U32(newLayout));
    }

    const auto& trans = it->second;

    VkImageMemoryBarrier barrier {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .srcAccessMask = trans.srcAccessMask,
        .dstAccessMask = trans.dstAccessMask,
        .oldLayout = currentLayout,
        .newLayout = newLayout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = pImage->GetHandle(),
        .subresourceRange = {
            .aspectMask = aspectFlags,
            .baseMipLevel = baseMipLevel,
            .levelCount = levelCount,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
    };

    vkCmdPipelineBarrier(
        m_pHandle,
        trans.sourceStage,
        trans.destinationStage,
        0,
        0,
        nullptr,
        0,
        nullptr,
        1,
        &barrier
    );
    pImage->SetCurrentImageLayout(newLayout);
}

void CommandBuffer::BindVertexBuffer(Buffer *pVertexBuffer, Buffer *pInstanceBuffer) const {
    std::vector<VkBuffer> buffers;
    std::vector<VkDeviceSize> offsets;

    buffers.push_back(pVertexBuffer->GetHandle());
    offsets.push_back(0);

    if (pInstanceBuffer) {
        buffers.push_back(pInstanceBuffer->GetHandle());
        offsets.push_back(0);
    }

    vkCmdBindVertexBuffers(m_pHandle, 0, buffers.size(), buffers.data(), offsets.data());
}

void CommandBuffer::BindIndexBuffer(Buffer *pIndexBuffer) const {
    vkCmdBindIndexBuffer(m_pHandle, pIndexBuffer->GetHandle(), 0, VK_INDEX_TYPE_UINT32);
}

void CommandBuffer::Draw(u32 vertexCount, u32 instanceCount) const {
	vkCmdDraw(m_pHandle, vertexCount, instanceCount, 0, 0);
}

void CommandBuffer::DrawIndex(u32 indexCount, u32 instanceCount) const {
    vkCmdDrawIndexed(m_pHandle, indexCount, instanceCount, 0, 0, 0);
}

void CommandBuffer::NextSubpass() const {
    vkCmdNextSubpass(m_pHandle, VK_SUBPASS_CONTENTS_INLINE);
}

void CommandBuffer::EndRenderPass() const {
    vkCmdEndRenderPass(m_pHandle);
}

void CommandBuffer::End() const {
    CALL_VK(vkEndCommandBuffer(m_pHandle));
}

USING_ENGINE_NAMESPACE_END
