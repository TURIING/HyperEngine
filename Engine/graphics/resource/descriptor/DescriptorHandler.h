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
    explicit DescriptorHandler(Pipeline* pPipeline);
    ~DescriptorHandler();

    template<typename T, typename = std::enable_if_t<std::is_base_of_v<Descriptor, T>>>
    void Push(const std::string &descriptorName, T *descriptor, const std::optional<OffsetSize> &offsetSize = std::nullopt) {
        if (!m_pShader) return;
        auto it = m_mapDescriptor.find(descriptorName);
        if (it != m_mapDescriptor.end()) {
            if (it->second.pDescriptor == std::to_address(descriptor) && it->second.offsetSize == offsetSize) {
                return;
            }
            m_mapDescriptor.erase(it);
        }

        if (!to_address(descriptor)) return;

        auto location = m_pShader->GetDescriptorLocation(descriptorName);
        if (!location) {
            return;
        }

        auto descriptorType = m_pShader->GetDescriptorType(*location);
        if (!descriptorType) return;

        auto writeDescriptor = descriptor->GetWriteDescriptorSet(*location, *descriptorType, offsetSize);
        m_mapDescriptor.emplace(descriptorName, DescriptorValue{ to_address(descriptor), std::move(writeDescriptor), offsetSize, *location });
        m_changed = true;
    }

    template<typename T>
    void Push(const std::string &descriptorName, const T &descriptor, WriteDescriptorSet writeDescriptorSet) {
        if (!m_pShader) return;
        if (auto it = m_mapDescriptor.find(descriptorName); it != m_mapDescriptor.end()) {
            m_mapDescriptor.erase(it);
        }

        auto location = m_pShader->GetDescriptorLocation(descriptorName);
        m_mapDescriptor.emplace(descriptorName, DescriptorValue{ to_address(descriptor), std::move(writeDescriptorSet), *location });
        m_changed = true;
    }

    void Push(const std::string &descriptorName, UniformHandler &uniformHandler, const std::optional<OffsetSize> &offsetSize = std::nullopt);
    bool Update(Pipeline* pPipeline);
    void BindDescriptor(CommandBuffer* pCommandBuffer, Pipeline* pPipeline) const;

private:
    class DescriptorValue {
    public:
        const Descriptor* pDescriptor = nullptr;
        WriteDescriptorSet writeDescriptor;
        std::optional<OffsetSize> offsetSize;
        uint32_t location;
    };
    const Shader* m_pShader = nullptr;
    Unique<DescriptorSet> m_pDescriptorSet = nullptr;
    std::map<std::string, DescriptorValue> m_mapDescriptor;
    std::vector<VkWriteDescriptorSet> m_vecWriteDescriptorSet;
    bool m_pushDescriptors = false;
    bool m_changed = false;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_DESCRIPTORHANDLER_H