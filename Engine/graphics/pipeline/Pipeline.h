//
// Created by turiing on 2026/1/14.
//

#ifndef HYPERENGINE_PIPELINE_H
#define HYPERENGINE_PIPELINE_H

#include "../../common/Common.h"
#include "../VulObject.h"
#include "../command/CommandBuffer.h"
#include "Shader.h"

USING_ENGINE_NAMESPACE_BEGIN
struct GraphicPipelineState
{
    VkPipelineLayout pipeLineLayout{};
    VkRenderPass renderPass{};
    std::vector<VkPipelineShaderStageCreateInfo> shaderStages;
    VkPipelineVertexInputStateCreateInfo vertexInputState{};
    VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
    VkPipelineViewportStateCreateInfo viewportState{};
    VkPipelineRasterizationStateCreateInfo rasterizationState{};
    VkPipelineMultisampleStateCreateInfo multiSampleState{};
    VkPipelineDepthStencilStateCreateInfo depthStencilState{};
    VkPipelineColorBlendStateCreateInfo colorBlendState{};
    VkPipelineDynamicStateCreateInfo dynamicState{};
    uint32_t subPassIndex{ 0 };
};

struct ComputePipelineState {
    VkPipelineLayout pipeLineLayout{};
    VkPipelineShaderStageCreateInfo shaderStage{};
};

enum class PipelineType { Graphic, Compute };
struct PipelineCreateInfo {
    GraphicPipelineState* graphicPipelineState = nullptr;
    ComputePipelineState* computePipelineState = nullptr;
    PipelineType type = PipelineType::Graphic;
};

struct EnvInfo {
	std::vector<std::filesystem::path> shaders;
};

class Pipeline: public VulObject<VkPipeline> {
public:
    struct Stage {
        u32 renderStageIndex;
        u32 subpassIndex;

        bool operator==(const Stage& rhs) const {
            return renderStageIndex == rhs.renderStageIndex && subpassIndex == rhs.subpassIndex;
        }

        bool operator!=(const Stage& rhs) const {
            return !operator==(rhs);
        }

        bool operator<(const Stage& rhs) const {
            return renderStageIndex < rhs.renderStageIndex;
        }

        bool operator>(const Stage& rhs) const {
            return renderStageIndex > rhs.renderStageIndex;
        }
    };

    Pipeline() = default;
    ~Pipeline() override;
    NODISCARD VkPipelineBindPoint GetBindPoint() const;

protected:
    void Create(const PipelineCreateInfo &info);

private:
    PipelineType m_pipelineType;
};

struct PipelineInputAssemblyState
{
    PrimitiveType primitiveType;

    [[nodiscard]] static VkPrimitiveTopology transformPrimitiveTypeToVk(PrimitiveType primitiveType) {
        switch (primitiveType) {
            CASE_FROM_TO(PrimitiveType::POINT, VK_PRIMITIVE_TOPOLOGY_POINT_LIST)
            CASE_FROM_TO(PrimitiveType::LINE, VK_PRIMITIVE_TOPOLOGY_LINE_LIST)
            CASE_FROM_TO(PrimitiveType::LINE_STRIP, VK_PRIMITIVE_TOPOLOGY_LINE_STRIP)
            CASE_FROM_TO(PrimitiveType::TRIANGLE, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST)
            CASE_FROM_TO(PrimitiveType::TRIANGLE_STRIP, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP)
            default:    LOG_ASSERT(false);
        }
    }

    [[nodiscard]] VkPipelineInputAssemblyStateCreateInfo GetCreateInfo() const {
        VkPipelineInputAssemblyStateCreateInfo inputAssemblyState{};
        inputAssemblyState.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
        inputAssemblyState.topology = transformPrimitiveTypeToVk(primitiveType);
        inputAssemblyState.primitiveRestartEnable = VK_FALSE;
        return inputAssemblyState;
    }
};

struct PipelineViewportState
{
    static VkPipelineViewportStateCreateInfo GetCreateInfo()
    {
        VkPipelineViewportStateCreateInfo viewportState{};
        viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
        viewportState.viewportCount = 1;
        viewportState.scissorCount = 1;
        return viewportState;
    }
};

struct PipelineRasterizationState
{
    VkPolygonMode polygonMode = VK_POLYGON_MODE_FILL;
    VkCullModeFlags cullMode = VK_CULL_MODE_BACK_BIT;
    float lineWidth = 1.0f;
    VkFrontFace frontFace = VK_FRONT_FACE_CLOCKWISE;

    PipelineRasterizationState(const RasterizationInfo &rasterizationInfo) {
        this->polygonMode = static_cast<VkPolygonMode>(rasterizationInfo.polygonMode);
        this->cullMode = static_cast<VkCullModeFlags>(rasterizationInfo.cullMode);
        this->lineWidth = rasterizationInfo.lineWidth;
        this->frontFace = static_cast<VkFrontFace>(rasterizationInfo.frontFace);
    }

    [[nodiscard]] VkPipelineRasterizationStateCreateInfo GetCreateInfo() const {
        VkPipelineRasterizationStateCreateInfo rasterizationState{};
        rasterizationState.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
        rasterizationState.depthClampEnable = VK_FALSE;
        rasterizationState.rasterizerDiscardEnable = VK_FALSE;
        rasterizationState.polygonMode = polygonMode;
        rasterizationState.lineWidth = lineWidth;
        rasterizationState.cullMode = cullMode;
        rasterizationState.frontFace = frontFace;
        rasterizationState.depthBiasEnable = VK_FALSE;
        return rasterizationState;
    }
};

struct PipelineMultiSampleState
{
    bool enable = false;
    VkSampleCountFlagBits sampleCount = VK_SAMPLE_COUNT_1_BIT;

	PipelineMultiSampleState(const MultiSampleInfo &multiSampleInfo) {
		this->enable = multiSampleInfo.enable;
		this->sampleCount = static_cast<VkSampleCountFlagBits>(multiSampleInfo.sampleCount);
	}

	[[nodiscard]] VkPipelineMultisampleStateCreateInfo GetCreateInfo() const {
        VkPipelineMultisampleStateCreateInfo multiSampleState{};
        multiSampleState.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
        multiSampleState.sampleShadingEnable = enable ? VK_TRUE : VK_FALSE;
        multiSampleState.rasterizationSamples = sampleCount;
    	multiSampleState.minSampleShading = 1.0f; // 仅在 sampleShadingEnable = VK_TRUE 有效
    	multiSampleState.pSampleMask = nullptr;
    	multiSampleState.alphaToCoverageEnable = VK_FALSE;
    	multiSampleState.alphaToOneEnable = VK_FALSE;
        return multiSampleState;
    }
};

struct PipelineColorBlendState
{
    bool enable = false;
    std::vector<VkPipelineColorBlendAttachmentState> vecColorBlendAttachmentState;
	Color constantColor;

	PipelineColorBlendState(const std::vector<BlendInfo> &infos) {
	    for (auto &info: infos) {
	        vecColorBlendAttachmentState.emplace_back(VkPipelineColorBlendAttachmentState {
                .blendEnable = info.enable ? VK_TRUE : VK_FALSE,
                .srcColorBlendFactor = gBlendFactorToVkBlendFactor[TO_I32(info.srcColorBlendFactor)],
                .dstColorBlendFactor = gBlendFactorToVkBlendFactor[TO_I32(info.dstColorBlendFactor)],
                .colorBlendOp = gBlendOpToVkBlendOp[TO_I32(info.colorBlendOp)],
                .srcAlphaBlendFactor = gBlendFactorToVkBlendFactor[TO_I32(info.srcAlphaBlendFactor)],
                .dstAlphaBlendFactor = gBlendFactorToVkBlendFactor[TO_I32(info.dstAlphaBlendFactor)],
                .alphaBlendOp = gBlendOpToVkBlendOp[TO_I32(info.alphaBlendOp)],
                .colorWriteMask	  = static_cast<VkColorComponentFlags>(info.colorWriteMask),
            });
	        constantColor = info.constantColor;
	    }
	}

    NODISCARD VkPipelineColorBlendStateCreateInfo GetCreateInfo() const {
		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType				= VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable		= VK_FALSE;
		colorBlending.logicOp			= VK_LOGIC_OP_COPY;
		colorBlending.attachmentCount	= vecColorBlendAttachmentState.size();
		colorBlending.pAttachments		= vecColorBlendAttachmentState.data();
		colorBlending.blendConstants[0] = constantColor.r;
		colorBlending.blendConstants[1] = constantColor.g;
		colorBlending.blendConstants[2] = constantColor.b;
		colorBlending.blendConstants[3] = constantColor.a;
		return colorBlending;
	}
};

struct PipelineDynamicState
{
    std::vector<VkDynamicState> dynamicStates = {
        VK_DYNAMIC_STATE_VIEWPORT,      //  允许通过vkCmdSetViewport动态修改视口
        VK_DYNAMIC_STATE_SCISSOR
    };

    [[nodiscard]] VkPipelineDynamicStateCreateInfo GetCreateInfo() const {
        VkPipelineDynamicStateCreateInfo dynamicState{};
        dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
        dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates	   = dynamicStates.data();
		return dynamicState;
	}
};

struct PipelineDepthStencilState {
	bool		depthTestEnable	  = false;
	bool		depthWriteEnable  = false;
	VkCompareOp depthOpFunc		  = VK_COMPARE_OP_LESS;
	bool		stencilTestEnable = false;

	explicit PipelineDepthStencilState(const DepthStencilInfo &info) {
		depthTestEnable = info.enableDepthTest;
		depthWriteEnable = info.enableDepthWrite;
		depthOpFunc = static_cast<VkCompareOp>(info.depthCompareOp);
		stencilTestEnable = info.enableStencilTest;
	}

	[[nodiscard]] VkPipelineDepthStencilStateCreateInfo GetCreateInfo() const {
		const VkPipelineDepthStencilStateCreateInfo depthStencilState{
			.sType				   = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
			.depthTestEnable	   = depthTestEnable ? VK_TRUE : VK_FALSE,
			.depthWriteEnable	   = depthWriteEnable ? VK_TRUE : VK_FALSE,
			.depthCompareOp		   = depthOpFunc,
			.depthBoundsTestEnable = VK_FALSE,
			.stencilTestEnable	   = stencilTestEnable ? VK_TRUE : VK_FALSE,
			.minDepthBounds		   = 0.0f,
			.maxDepthBounds		   = 1.0f,
		};
		return depthStencilState;
	}
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_PIPELINE_H