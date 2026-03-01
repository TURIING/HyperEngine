//
// Created by turiing on 2026/1/17.
//

#include "DescriptorHandler.h"
#include "../../Graphics.h"
#include "../../device/LogicDevice.h"

USING_ENGINE_NAMESPACE_BEGIN
DescriptorHandler::~DescriptorHandler() {
}

void DescriptorHandler::Push(const std::string &descriptorName, UniformHandler &uniformHandler, const std::optional<OffsetSize> &offsetSize) {
    if (!m_pShader) return;
    uniformHandler.BindUniformBlock(m_pShader->GetUniformBlock(descriptorName));
    Push(descriptorName, uniformHandler.GetUniformBuffer(), offsetSize);
}

void DescriptorHandler::Update() {
    if (!m_changed) return;

    m_vecWriteDescriptorSet.clear();
    m_vecWriteDescriptorSet.reserve(m_mapNameToDescriptorValue.size());

    for (const auto &[descriptorName, descriptor] : m_mapNameToDescriptorValue) {
        auto writeDescriptorSet = descriptor.writeDescriptor.GetWriteDescriptorSet();
        writeDescriptorSet.dstSet = m_pDescriptorSet->GetHandle();
        m_vecWriteDescriptorSet.emplace_back(writeDescriptorSet);
    }
    DescriptorSet::Update(m_vecWriteDescriptorSet);

    m_changed = false;
}

void DescriptorHandler::BindDescriptor(const CommandBuffer *pCommandBuffer) const {
    pCommandBuffer->BindDescriptor(m_pPipeline, m_pDescriptorSet.get());
}

void DescriptorHandler::BindPipeline(Pipeline *pipeline) {
    if (const auto pShader = pipeline->GetShader(); pShader != m_pShader) {
        m_pPipeline = pipeline;
        m_pShader = pShader;
        m_pDescriptorSet = std::make_unique<DescriptorSet>(pipeline);
        m_mapNameToDescriptorValue.clear();
        m_vecWriteDescriptorSet.clear();
        m_changed = false;
    }
}

USING_ENGINE_NAMESPACE_END
