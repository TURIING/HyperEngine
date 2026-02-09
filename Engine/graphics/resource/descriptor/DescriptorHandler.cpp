//
// Created by turiing on 2026/1/17.
//

#include "DescriptorHandler.h"
#include "../../Graphics.h"
#include "../../device/LogicDevice.h"

USING_ENGINE_NAMESPACE_BEGIN
DescriptorHandler::DescriptorHandler(Pipeline *pPipeline) {
}

DescriptorHandler::~DescriptorHandler() {
}

void DescriptorHandler::Push(const std::string &descriptorName, UniformHandler &uniformHandler, const std::optional<OffsetSize> &offsetSize) {
    if (!m_pShader) return;
    uniformHandler.Update(m_pShader->GetUniformBlock(descriptorName));
    Push(descriptorName, uniformHandler.GetUniformBuffer(), offsetSize);
}

bool DescriptorHandler::Update(Pipeline *pPipeline) {
    // if (m_pShader != pPipeline->GetShader()) {
    //     m_pShader = pPipeline->GetShader();
    //     m_pushDescriptors = pPipeline->IsPushDescriptors();
    //     m_mapDescriptor.clear();
    //     m_vecWriteDescriptorSet.clear();
    //
    //     if (!m_pushDescriptors) {
    //         m_pDescriptorSet = std::make_unique<DescriptorSet>(pPipeline);
    //     }
    //
    //     m_changed = false;
    //     return false;
    // }

    // if (m_changed) {
    //     m_vecWriteDescriptorSet.clear();
    //     m_vecWriteDescriptorSet.reserve(m_mapDescriptor.size());
    //
    //     for (const auto &[descriptorName, descriptor] : m_mapDescriptor) {
    //         auto writeDescriptorSet = descriptor.writeDescriptor.GetWriteDescriptorSet();
    //         writeDescriptorSet.dstSet = VK_NULL_HANDLE;
    //
    //         if (!m_pushDescriptors)
    //             writeDescriptorSet.dstSet = m_pDescriptorSet->GetHandle();
    //
    //         m_vecWriteDescriptorSet.emplace_back(writeDescriptorSet);
    //     }
    //
    //     if (!m_pushDescriptors)
    //         DescriptorSet::Update(m_vecWriteDescriptorSet);
    //
    //     m_changed = false;
    // }

    return true;
}

void DescriptorHandler::BindDescriptor(CommandBuffer *pCommandBuffer, Pipeline *pPipeline) const {
    if (m_pushDescriptors) {
        auto logicalDevice = Graphics::Get()->GetLogicDevice();
        // vkCmdPushDescriptorSetKHR(pCommandBuffer->GetHandle(), pPipeline->GetPipelineBindPoint(), pPipeline->GetPipelineLayout(), 0,
        //     static_cast<uint32_t>(m_vecWriteDescriptorSet.size()), m_vecWriteDescriptorSet.data());
    } else {
        m_pDescriptorSet->BindDescriptor(pCommandBuffer);
    }
}

USING_ENGINE_NAMESPACE_END
