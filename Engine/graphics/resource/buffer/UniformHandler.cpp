//
// Created by turiing on 2026/1/13.
//

#include "UniformHandler.h"


USING_ENGINE_NAMESPACE_BEGIN
UniformHandler::UniformHandler(bool multiPipeline): m_multiPipeline(multiPipeline), m_handlerStatus(Buffer::Status::Normal) {
}

UniformHandler::UniformHandler(const Shader::UniformBlock &uniformBlock, bool multiPipeline)
    :m_multiPipeline(multiPipeline), m_uniformBlock(uniformBlock), m_size(m_uniformBlock->size),
     m_uniformBuffer(std::make_unique<UniformBuffer>(m_size)), m_handlerStatus(Buffer::Status::Normal){
}

bool UniformHandler::Update(const std::optional<Shader::UniformBlock> &uniformBlock) {
    if (m_handlerStatus == Buffer::Status::Reset || (m_multiPipeline && !m_uniformBlock) || (!m_multiPipeline && m_uniformBlock != uniformBlock)) {
        if ((m_size == 0 && !m_uniformBlock) || (m_uniformBlock && m_uniformBlock != uniformBlock && m_uniformBlock->size == m_size)) {
            m_size = m_uniformBlock->size;
        }

        m_uniformBlock = uniformBlock;
        m_bound = false;
        m_uniformBuffer = std::make_unique<UniformBuffer>(m_size);
        m_handlerStatus = Buffer::Status::Changed;
        return false;
    }

    if (m_handlerStatus == Buffer::Status::Normal) {
        if (m_bound) {
            m_uniformBuffer->Unmap();
            m_bound = false;
        }
        m_handlerStatus = Buffer::Status::Normal;
    }
    return true;
}

USING_ENGINE_NAMESPACE_END
