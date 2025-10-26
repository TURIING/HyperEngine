//
// Created by turiing on 2025/10/18.
//

#ifndef MODEL_HPP
#define MODEL_HPP

#include "../common/Common.h"
#include "ModelFactory.hpp"
#include "../resource/Resource.h"
#include "HyperGpu.h"
#include "../graphics/Graphics.h"

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
    const std::vector<HyperGpu::VertexAttribute> vertexAttributes = {
        { 0, HyperGpu::AttributeDataType::Vec3 },
        {1, HyperGpu::AttributeDataType::Vec2 },
        { 2, HyperGpu::AttributeDataType::Vec2 }
    };
public:
    Model() = default;

    template<typename T>
    explicit Model(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {}) {
        initialize<T>(vertices, indices);
    }

    NODISCARD std::type_index GetTypeIndex() const override { return typeid(Model); }

protected:
    template<typename T>
    void initialize(const std::vector<T> &vertices, const std::vector<uint32_t> &indices = {}) {
        auto pGpuDevice = Graphics::Instance()->GetGpuDevice();
        HyperGpu::InputAssemblerInfo info;
        info.attributeCount = static_cast<uint32_t>(vertexAttributes.size());
        info.pAttributes = vertexAttributes.data();
        info.indexCount = static_cast<uint32_t>(indices.size());
        info.pIndexData = indices.data();
        info.indexSize = sizeof(uint32_t) * indices.size();
        info.vertexCount = static_cast<uint32_t>(vertices.size());
        info.pVertexData = vertices.data();
        info.vertexSize = sizeof(T) * vertices.size();
        m_pInputAssembler = pGpuDevice->GetResourceManager()->CreateInputAssembler(info);
    }

private:
    HyperGpu::InputAssembler *m_pInputAssembler = nullptr;
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
