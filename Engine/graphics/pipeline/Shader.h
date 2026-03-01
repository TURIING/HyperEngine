//
// Created by turiing on 2026/1/13.
//

#ifndef HYPERENGINE_SHADER_H
#define HYPERENGINE_SHADER_H

#include <utility>

#include "../../common/Common.h"

namespace glslang {
    class TProgram;
    class TType;
}

USING_ENGINE_NAMESPACE_BEGIN
struct PipelineVertexInputState
{
    std::vector<VkVertexInputBindingDescription> vecBindingDescription;
    std::vector<VkVertexInputAttributeDescription> attributeDescriptions;
    NODISCARD VkPipelineVertexInputStateCreateInfo GetCreateInfo() const {
        VkPipelineVertexInputStateCreateInfo vertexInputState{};
        vertexInputState.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
        vertexInputState.vertexBindingDescriptionCount = vecBindingDescription.size();
        vertexInputState.pVertexBindingDescriptions = vecBindingDescription.data();
        vertexInputState.vertexAttributeDescriptionCount = attributeDescriptions.size();
        vertexInputState.pVertexAttributeDescriptions = attributeDescriptions.data();
        return vertexInputState;
    }
};

class Shader {
public:
    struct Uniform {
        int32_t binding;
        int32_t offset;
        int32_t size;
        int32_t glType;
        bool readOnly;
        bool writeOnly;
        VkShaderStageFlags stageFlags;

        Uniform(int32_t binding = -1, int32_t offset = -1, int32_t size = -1, int32_t glType = -1, bool readOnly = false, bool writeOnly = false, VkShaderStageFlags stageFlags = 0)
            :binding(binding), offset(offset), size(size), glType(glType), readOnly(readOnly), writeOnly(writeOnly), stageFlags(stageFlags) {}

        bool operator==(const Uniform& other) const {
            return binding == other.binding && offset == other.offset && size == other.size && glType == other.glType &&
                readOnly == other.readOnly && writeOnly == other.writeOnly && stageFlags == other.stageFlags;
        }

        bool operator!=(const Uniform& other) const {
            return !operator==(other);
        }
    };

    struct UniformBlock {
        enum class Type { None, Uniform, Storage, PushConstant };
        int32_t binding;
        int32_t size;
        VkShaderStageFlags stageFlags;
        Type type;

        UniformBlock(int32_t binding = -1, int32_t size = -1, VkShaderStageFlags stageFlags = 0, Type type = Type::Uniform)
            :binding(binding), size(size), stageFlags(stageFlags), type(type) {}

        NODISCARD std::optional<Uniform> GetUniform(const std::string &name) const {
            auto it = m_uniforms.find(name);
            if(it == m_uniforms.end()) {
                return std::nullopt;
            }
            return it->second;
        }

        bool operator==(const UniformBlock &other) const {
            return binding == other.binding && size == other.size && stageFlags == other.stageFlags && m_uniforms == other.m_uniforms && type == other.type;
        }

        bool operator!=(const UniformBlock &other) const {
            return !operator==(other);
        }

        Uniform& operator[](const std::string& name) {
            return m_uniforms[name];
        }

    private:
        std::map<std::string, Uniform> m_uniforms;
    };

    using Define = std::pair<std::string, std::string>;

    struct VertexInput {
        uint32_t binding = 0;
        std::vector<VkVertexInputBindingDescription> bindingDescriptions;
        std::vector<VkVertexInputAttributeDescription> attributeDescriptions;

        VertexInput(std::vector<VkVertexInputBindingDescription> bindingDescriptions = {}, std::vector<VkVertexInputAttributeDescription> attributeDescriptions = {})
            :bindingDescriptions(std::move(bindingDescriptions)), attributeDescriptions(std::move(attributeDescriptions)) {}

        bool operator<(const VertexInput &rhs) const {
            return bindingDescriptions.front().binding < rhs.bindingDescriptions.front().binding;
        }
    };

    struct Attribute {
        std::string name;
        int32_t location;
        int32_t size;
        int32_t glType;
        int32_t set;

        Attribute(std::string name, int32_t set = -1, int32_t location = -1, int32_t size = -1, int32_t glType = -1)
            :location(location), size(size), glType(glType), set(set), name(std::move(name)) {}

        NODISCARD bool operator==(const Attribute &rhs) const {
            return location == rhs.location && name == rhs.name && size == rhs.size && glType == rhs.glType && set == rhs.set;
        }

        NODISCARD bool operator!=(const Attribute &rhs) const {
            return !operator==(rhs);
        }
    };

    struct Constant {
        int32_t binding;
        int32_t size;
        int32_t glType;
        VkShaderStageFlags stageFlags;

        Constant(int32_t binding = -1, int32_t size = -1, int32_t glType = -1, VkShaderStageFlags stageFlags = 0)
            :binding(binding),size(size),glType(glType),stageFlags(stageFlags) {}

        NODISCARD bool operator==(const Constant &rhs) const {
			return binding == rhs.binding && size == rhs.size && stageFlags == rhs.stageFlags && glType == rhs.glType;
        }

        NODISCARD bool operator!=(const Constant &rhs) const {
            return !operator==(rhs);
        }
    };

    Shader(const std::vector<std::filesystem::path> &files, const std::optional<std::vector<Define>> &defines = std::nullopt);
    NODISCARD const PipelineVertexInputState &GetVertexInputState() const { return m_vertexInputState; }
    NODISCARD const std::vector<VkDescriptorSetLayoutBinding> &GetDescriptorSetLayoutBindings() const { return m_vecDescriptorSetLayoutBinding; }
    NODISCARD const std::vector<VkPushConstantRange> &GetPushConstantRanges() const { return m_vecPushConstantRanges; }
    NODISCARD const std::vector<VkPipelineShaderStageCreateInfo> &GetShaderStages() const { return m_vecShaderStages; }
    NODISCARD std::optional<uint32_t> GetDescriptorLocation(const std::string &name) const;
    NODISCARD std::optional<VkDescriptorType> GetDescriptorType(const std::string &name) const;
    NODISCARD std::optional<UniformBlock> GetUniformBlock(std::string_view name) const;
    NODISCARD std::optional<Uniform> GetUniform(std::string_view name) const;
    static VkFormat GlTypeToVk(int32_t type);

private:
    void loadUniformBlock(const glslang::TProgram &program, VkShaderStageFlags stageFlag, int32_t i);
    void loadUniform(const glslang::TProgram &program, VkShaderStageFlags stageFlag, int32_t i);
    void loadAttribute(const glslang::TProgram &program, VkShaderStageFlags stageFlag, int32_t i);
    static int32_t ComputeSize(const glslang::TType *ttype);
    static void IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, VkDescriptorType type);
    void compileAndReflect(std::string_view code, std::string_view defines, VkShaderStageFlags moduleFlag);
    void createAndPushShaderModule(const std::vector<uint32_t> &spirv);
    void createDescriptorSetLayoutBindings();
    void createVertexInputState();
    void createPushConstantRanges();

private:
    std::vector<std::filesystem::path> m_stages;
    std::map<std::string, Uniform> m_mapUniform;
    std::map<std::string, UniformBlock> m_mapUniformBlock;
    std::map<int32_t, Attribute> m_attributes;
    std::map<std::string, Constant> m_constants;

    std::array<std::optional<uint32_t>, 3> m_localSizes;

    std::map<std::string, uint32_t> m_mapDescriptorNameToLocation;
    std::map<std::string, uint32_t> m_descriptorSizes;

    uint32_t m_lastDescriptorBinding = 0;

    mutable std::vector<std::string> m_notFoundNames;

    std::vector<VkShaderModule> m_vecShaderModules;
    std::vector<VkPipelineShaderStageCreateInfo> m_vecShaderStages;
    std::vector<VkDescriptorSetLayoutBinding> m_vecDescriptorSetLayoutBinding;
    std::unordered_map<std::string, VkDescriptorType> m_mapNameToDescriptorType;
    PipelineVertexInputState m_vertexInputState;
    std::vector<VkPushConstantRange> m_vecPushConstantRanges;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_SHADER_H