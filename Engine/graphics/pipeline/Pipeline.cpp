//
// Created by turiing on 2026/2/5.
//
#include "Pipeline.h"
#include "../Graphics.h"
#include "../device/LogicDevice.h"

USING_ENGINE_NAMESPACE_BEGIN
void Pipeline::Create(const PipelineCreateInfo &info) {
    auto pLogicDevice = Graphics::Get()->GetLogicDevice();
    m_pipelineType = info.type;

    if (info.type == PipelineType::Graphic) {
        auto state = *info.graphicPipelineState;
        VkGraphicsPipelineCreateInfo pipelineCreateInfo {
            .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
            .stageCount = static_cast<uint32_t>(state.shaderStages.size()),
            .pStages = state.shaderStages.data(),
            .pVertexInputState = &state.vertexInputState,
            .pInputAssemblyState = &state.inputAssemblyState,
            .pViewportState = &state.viewportState,
            .pRasterizationState = &state.rasterizationState,
            .pMultisampleState = &state.multiSampleState,
            .pDepthStencilState = &state.depthStencilState,
            .pColorBlendState = &state.colorBlendState,
            .pDynamicState = &state.dynamicState,
            .layout = state.pipeLineLayout,
            .renderPass = state.renderPass,
            .subpass = state.subPassIndex,
            .basePipelineHandle = VK_NULL_HANDLE,
        };
        CALL_VK(vkCreateGraphicsPipelines(pLogicDevice->GetHandle(), nullptr, 1, &pipelineCreateInfo, nullptr, &m_pHandle));
        LOG_DEBUG("Graphic Pipeline created");
    }
    else {
        auto state = *info.computePipelineState;
        VkComputePipelineCreateInfo createInfo {
            .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
            .pNext = nullptr,
            .stage = state.shaderStage,
            .layout = state.pipeLineLayout,
        };
        CALL_VK(vkCreateComputePipelines(pLogicDevice->GetHandle(), VK_NULL_HANDLE, 1, &createInfo, nullptr, &m_pHandle));
        LOG_DEBUG("Compute Pipeline created");
    }
}

Pipeline::~Pipeline() {
    auto pLogicDevice = Graphics::Get()->GetLogicDevice();
    vkDestroyPipeline(pLogicDevice->GetHandle(), m_pHandle, nullptr);
}

VkPipelineBindPoint Pipeline::GetBindPoint() const {
    if (m_pipelineType == PipelineType::Graphic)
        return VK_PIPELINE_BIND_POINT_GRAPHICS;
    else
        return VK_PIPELINE_BIND_POINT_COMPUTE;
}

USING_ENGINE_NAMESPACE_END
