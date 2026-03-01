//
// Created by turiing on 2026/1/13.
//

#include "UniformHandler.h"
#include "../../pipeline/Pipeline.h"

USING_ENGINE_NAMESPACE_BEGIN
void UniformHandler::BindUniformBlock(std::optional<Shader::UniformBlock> block) {
    m_uniformBlock = std::move(block);
    m_handlerStatus = Buffer::Status::Reset;
    if (block) {
        m_size = block->size;
        m_uniformBuffer = std::make_unique<UniformBuffer>(m_size);
    }
}

void UniformHandler::Update() {
    if (m_bound) {
        m_uniformBuffer->Unmap();
        m_bound = false;
    }
    m_handlerStatus = Buffer::Status::Normal;

}

USING_ENGINE_NAMESPACE_END
