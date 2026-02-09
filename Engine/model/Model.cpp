//
// Created by turiing on 2025/10/18.
//

#include "Model.hpp"

USING_ENGINE_NAMESPACE_BEGIN
void Model::SetIndices(const std::vector<uint32_t> &indices) {
    if (indices.empty()) return;

    const auto size = sizeof(uint32_t) * indices.size();
    m_indexCount = indices.size();
    m_pIndexBuffer = std::make_unique<Buffer>(
        size,
        VK_BUFFER_USAGE_INDEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
        VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
    );
    Buffer stageBuffer(
        size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
    );
    stageBuffer.WriteData(0, size, indices.data());

    Graphics::Get()->GetCmdManager()->WithSingleCmdBuffer([&](CommandBuffer *pCmd) {
        pCmd->CopyBuffer(&stageBuffer, m_pIndexBuffer.get(), size);
    });
}

void Model::Render(CommandBuffer *pCmd) const {
    if (m_pVertexBuffer && m_pIndexBuffer) {
        pCmd->BindVertexBuffer(m_pVertexBuffer.get());
        pCmd->BindIndexBuffer(m_pIndexBuffer.get());
        pCmd->DrawIndex(m_indexCount);
    }
    else if (m_pVertexBuffer && !m_pIndexBuffer) {
        pCmd->BindVertexBuffer(m_pVertexBuffer.get());
        pCmd->Draw(m_vertexCount);
    }
    else {
        LOG_CRITICAL("Unexpected path!");
    }
}

USING_ENGINE_NAMESPACE_END
