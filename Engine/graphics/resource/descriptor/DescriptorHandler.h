//
// Created by turiing on 2026/1/17.
//

#ifndef HYPERENGINE_DESCRIPTORHANDLER_H
#define HYPERENGINE_DESCRIPTORHANDLER_H
#include "../../../common/Common.h"
#include "../../pipeline/Shader.h"
#include "../../pipeline/Pipeline.h"
#include "DescriptorSet.h"
#include "Descriptor.h"
#include "../buffer/UniformHandler.h"

USING_ENGINE_NAMESPACE_BEGIN
class CommandBuffer;

class DescriptorHandler {
public:
    DescriptorHandler() = default;
    ~DescriptorHandler();

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<Descriptor, T>>>
    void Push(const std::string &descriptorName, T *descriptor, const std::optional<OffsetSize> &offsetSize = std::nullopt) {
        LOG_ASSERT(m_pShader);
        LOG_ASSERT(std::to_address(descriptor));

        if (auto it = m_mapNameToDescriptorValue.find(descriptorName); it != m_mapNameToDescriptorValue.end()) {
            if (it->second.pDescriptor == std::to_address(descriptor)) {
                return;
            }
            m_mapNameToDescriptorValue.erase(it);
        }

        auto location = m_pShader->GetDescriptorLocation(descriptorName);
        LOG_ASSERT(location);

        auto descriptorType = m_pShader->GetDescriptorType(descriptorName);
        LOG_ASSERT(descriptorType);

        auto writeDescriptor = descriptor->GetWriteDescriptorSet(*location, *descriptorType);
        m_mapNameToDescriptorValue.emplace(descriptorName, DescriptorValue{ std::to_address(descriptor), std::move(writeDescriptor) });
        m_changed = true;
    }

    template<typename T>
    void Push(const std::string &descriptorName, const T &descriptor, WriteDescriptorSet writeDescriptorSet) {
        LOG_ASSERT(m_pShader);

        if (auto it = m_mapNameToDescriptorValue.find(descriptorName); it != m_mapNameToDescriptorValue.end()) {
            m_mapNameToDescriptorValue.erase(it);
        }

        auto location = m_pShader->GetDescriptorLocation(descriptorName);
        m_mapNameToDescriptorValue.emplace(descriptorName, DescriptorValue{ std::to_address(descriptor), std::move(writeDescriptorSet) });
        m_changed = true;
    }

    void Push(const std::string &descriptorName, UniformHandler &uniformHandler, const std::optional<OffsetSize> &offsetSize = std::nullopt);
    void Update();
    void BindDescriptor(const CommandBuffer* pCommandBuffer) const;
    void BindPipeline(Pipeline* pipeline);

private:
    struct DescriptorValue {
        const Descriptor* pDescriptor = nullptr;
        WriteDescriptorSet writeDescriptor;
    };

    const Shader* m_pShader = nullptr;
    const Pipeline* m_pPipeline = nullptr;
    Unique<DescriptorSet> m_pDescriptorSet = nullptr;
    std::map<std::string, DescriptorValue> m_mapNameToDescriptorValue;
    std::vector<VkWriteDescriptorSet> m_vecWriteDescriptorSet;
    bool m_changed = false;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_DESCRIPTORHANDLER_H