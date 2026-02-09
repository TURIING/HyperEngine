//
// Created by turiing on 2025/10/18.
//

#ifndef MODEL_HPP
#define MODEL_HPP

#include "../common/Common.h"
#include "ModelFactory.hpp"
#include "../resource/Resource.h"
#include "../graphics/Graphics.h"
#include "../graphics/resource/buffer/Buffer.h"
#include "../graphics/command/CmdManager.h"
#include "../graphics/command/CommandBuffer.h"

USING_ENGINE_NAMESPACE_BEGIN

struct Vertex {
    glm::vec3 position;
    glm::vec2 uv;
    glm::vec3 normal;

    bool operator==(const Vertex& other) const noexcept {
        return position == other.position &&
               uv == other.uv &&
               normal == other.normal;
    }
};

class ENGINE_EXPORT Model: public ModelFactory<Model>, public Resource {
public:
    Model() = default;

    template<typename T>
    explicit Model(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {}) {
        initialize<T>(vertices, indices);
    }

	void SetIndices(const std::vector<uint32_t> &indices);
    NODISCARD std::type_index GetTypeIndex() const override { return typeid(Model); }
    void Render(CommandBuffer* pCmd) const;

protected:
    template<typename T>
    void initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {}) {
        SetVertices(vertices);
        SetIndices(indices);
    }

    template<typename T>
    void SetVertices(const std::vector<T> &vertices) {
        LOG_ASSERT(!vertices.empty());
        const auto size = sizeof(T) * vertices.size();
        m_vertexCount = vertices.size();
        m_pVertexBuffer = std::make_unique<Buffer>(
            size,
            VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_SRC_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT,
            VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
        );
        Buffer stageBuffer(
            size,
            VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
            VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT
        );
        stageBuffer.WriteData(0, size, vertices.data());

        Graphics::Get()->GetCmdManager()->WithSingleCmdBuffer([&](CommandBuffer *pCmd) {
            pCmd->CopyBuffer(&stageBuffer, m_pVertexBuffer.get(), size);
        });
    }

private:
    u32 m_vertexCount = 0;
    u32 m_indexCount = 0;
    Unique<Buffer> m_pVertexBuffer;
    Unique<Buffer> m_pIndexBuffer;
};

USING_ENGINE_NAMESPACE_END

namespace std {
    template <>
    struct hash<HyperEngine::Vertex> {
        size_t operator()(const HyperEngine::Vertex &vertex) const noexcept {
            size_t result = 0;
            HASH_COMBINE(result, vertex.position.x);
            HASH_COMBINE(result, vertex.position.y);
            HASH_COMBINE(result, vertex.position.z);
            HASH_COMBINE(result, vertex.uv.x);
            HASH_COMBINE(result, vertex.uv.y);
            HASH_COMBINE(result, vertex.normal.x);
            HASH_COMBINE(result, vertex.normal.y);
            HASH_COMBINE(result, vertex.normal.z);
            return result;
        }
    };
}
#endif //MODEL_HPP
