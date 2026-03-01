//
// Created by turiing on 2026/1/14.
//

#include "GraphicsPipeline.h"
#include "../../file/FileManager.h"
#include "../Graphics.h"
#include "graphics/device/LogicDevice.h"
#include "Shader.h"
#include "PipelineLayout.h"
#include "RenderPass.h"
#include "RenderStage.h"

USING_ENGINE_NAMESPACE_BEGIN

GraphicsPipeline::GraphicsPipeline(const RenderEnvInfo &renderEnvInfo):
    Pipeline(renderEnvInfo.shaders, renderEnvInfo.defines),
    m_stage(renderEnvInfo.stage) {
    auto renderStage = Graphics::Get()->GetRenderStage(m_stage.renderStageIndex);
    std::vector<BlendInfo> vecBlendInfo;
    for (auto &attachment: renderStage->GetAttachments()) {
        vecBlendInfo.push_back(attachment.blendInfo);
    }

	PipelineInputAssemblyState pipelineInputAssembly{renderEnvInfo.rasterInfo.primitiveType};
	PipelineRasterizationState pipelineRasterizationState(renderEnvInfo.rasterInfo);
	PipelineMultiSampleState   pipelineMultiSampleState(renderEnvInfo.multiSampleInfo);
	PipelineColorBlendState	   pipelineColorBlendState(vecBlendInfo);
	PipelineDynamicState	   pipelineDynamicState;
	PipelineDepthStencilState  pipelineDepthStencilState(renderEnvInfo.depthStencilInfo);

    GraphicPipelineState state {
        .pipeLineLayout = m_pPipelineLayout->GetHandle(),
        .renderPass = renderStage->GetRenderPass()->GetHandle(),
        .shaderStages = m_pShader->GetShaderStages(),
        .vertexInputState = m_pShader->GetVertexInputState().GetCreateInfo(),
        .inputAssemblyState = pipelineInputAssembly.GetCreateInfo(),
        .viewportState = PipelineViewportState::GetCreateInfo(),
        .rasterizationState = pipelineRasterizationState.GetCreateInfo(),
        .multiSampleState = pipelineMultiSampleState.GetCreateInfo(),
        .depthStencilState = pipelineDepthStencilState.GetCreateInfo(),
        .colorBlendState = pipelineColorBlendState.GetCreateInfo(),
        .dynamicState = pipelineDynamicState.GetCreateInfo(),
        .subPassIndex = m_stage.subpassIndex,
    };
    PipelineCreateInfo info;
    info.type = PipelineType::Graphic;
    info.graphicPipelineState = &state;
    create(info);
}

GraphicsPipeline::~GraphicsPipeline() {
}

USING_ENGINE_NAMESPACE_END
