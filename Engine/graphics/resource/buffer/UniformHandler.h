//
// Created by turiing on 2026/1/13.
//

#ifndef HYPERENGINE_UNIFORMHANDLER_H
#define HYPERENGINE_UNIFORMHANDLER_H

#include "../../../common/Common.h"
#include "../../pipeline/Shader.h"
#include "UniformBuffer.h"

USING_ENGINE_NAMESPACE_BEGIN
class Pipeline;

class UniformHandler {
public:
    template<typename T>
    void Push(const T &obj, std::size_t offset, std::size_t size) {
        LOG_ASSERT(m_uniformBlock && m_uniformBuffer);
        if (!m_bound) {
            m_uniformBuffer->Map(&m_pData);
            m_bound = true;
        }
        if (m_handlerStatus == Buffer::Status::Changed || std::memcmp(static_cast<char*>(m_pData), &obj, size) != 0) {
            std::memcpy(static_cast<char*>(m_pData) + offset, &obj, size);
            m_handlerStatus = Buffer::Status::Changed;
        }
    }

    template<typename T>
    void Push(const std::string &uniformName, const T &obj, std::size_t size = 0) {
        if (!m_uniformBlock || !m_uniformBuffer) return;
        auto uniform = m_uniformBlock->GetUniform(uniformName);
        if(!uniform) return;

        auto realSize = size;
        if (realSize == 0) {
            realSize = std::min(sizeof(obj), static_cast<std::size_t>(uniform->size));
        }
        Push(obj, uniform->size, realSize);
    }

    void BindUniformBlock(std::optional<Shader::UniformBlock> block);
    void Update();
    NODISCARD UniformBuffer* GetUniformBuffer() const { return m_uniformBuffer.get(); }

private:
    std::optional<Shader::UniformBlock> m_uniformBlock;
    uint32_t m_size = 0;
    void *m_pData = nullptr;
    bool m_bound = false;
    Unique<UniformBuffer> m_uniformBuffer;
    Buffer::Status m_handlerStatus {};
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_UNIFORMHANDLER_H