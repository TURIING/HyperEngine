//
// Created by turiing on 2026/1/13.
//

#include "Shader.h"

#include <ranges>
#include <SPIRV/GlslangToSpv.h>
#include <glslang/Public/ShaderLang.h>
#include <glslang/Include/Types.h>
#include "../Graphics.h"
#include "../../file/FileManager.h"
#include "graphics/device/LogicDevice.h"
#include "../resource/buffer/UniformBuffer.h"

USING_ENGINE_NAMESPACE_BEGIN
EShLanguage GetEshLanguage(VkShaderStageFlags stageFlag) {
	switch (stageFlag) {
		case VK_SHADER_STAGE_COMPUTE_BIT:					return EShLangCompute;
		case VK_SHADER_STAGE_VERTEX_BIT:					return EShLangVertex;
		case VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT:		return EShLangTessControl;
		case VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT:	return EShLangTessEvaluation;
		case VK_SHADER_STAGE_GEOMETRY_BIT:					return EShLangGeometry;
		case VK_SHADER_STAGE_FRAGMENT_BIT:					return EShLangFragment;
		default:											return EShLangCount;
	}
}

constexpr TBuiltInResource GetResources() {
	TBuiltInResource resources = {};
	resources.maxLights = 32;
	resources.maxClipPlanes = 6;
	resources.maxTextureUnits = 32;
	resources.maxTextureCoords = 32;
	resources.maxVertexAttribs = 64;
	resources.maxVertexUniformComponents = 4096;
	resources.maxVaryingFloats = 64;
	resources.maxVertexTextureImageUnits = 32;
	resources.maxCombinedTextureImageUnits = 80;
	resources.maxTextureImageUnits = 32;
	resources.maxFragmentUniformComponents = 4096;
	resources.maxDrawBuffers = 32;
	resources.maxVertexUniformVectors = 128;
	resources.maxVaryingVectors = 8;
	resources.maxFragmentUniformVectors = 16;
	resources.maxVertexOutputVectors = 16;
	resources.maxFragmentInputVectors = 15;
	resources.minProgramTexelOffset = -8;
	resources.maxProgramTexelOffset = 7;
	resources.maxClipDistances = 8;
	resources.maxComputeWorkGroupCountX = 65535;
	resources.maxComputeWorkGroupCountY = 65535;
	resources.maxComputeWorkGroupCountZ = 65535;
	resources.maxComputeWorkGroupSizeX = 1024;
	resources.maxComputeWorkGroupSizeY = 1024;
	resources.maxComputeWorkGroupSizeZ = 64;
	resources.maxComputeUniformComponents = 1024;
	resources.maxComputeTextureImageUnits = 16;
	resources.maxComputeImageUniforms = 8;
	resources.maxComputeAtomicCounters = 8;
	resources.maxComputeAtomicCounterBuffers = 1;
	resources.maxVaryingComponents = 60;
	resources.maxVertexOutputComponents = 64;
	resources.maxGeometryInputComponents = 64;
	resources.maxGeometryOutputComponents = 128;
	resources.maxFragmentInputComponents = 128;
	resources.maxImageUnits = 8;
	resources.maxCombinedImageUnitsAndFragmentOutputs = 8;
	resources.maxCombinedShaderOutputResources = 8;
	resources.maxImageSamples = 0;
	resources.maxVertexImageUniforms = 0;
	resources.maxTessControlImageUniforms = 0;
	resources.maxTessEvaluationImageUniforms = 0;
	resources.maxGeometryImageUniforms = 0;
	resources.maxFragmentImageUniforms = 8;
	resources.maxCombinedImageUniforms = 8;
	resources.maxGeometryTextureImageUnits = 16;
	resources.maxGeometryOutputVertices = 256;
	resources.maxGeometryTotalOutputComponents = 1024;
	resources.maxGeometryUniformComponents = 1024;
	resources.maxGeometryVaryingComponents = 64;
	resources.maxTessControlInputComponents = 128;
	resources.maxTessControlOutputComponents = 128;
	resources.maxTessControlTextureImageUnits = 16;
	resources.maxTessControlUniformComponents = 1024;
	resources.maxTessControlTotalOutputComponents = 4096;
	resources.maxTessEvaluationInputComponents = 128;
	resources.maxTessEvaluationOutputComponents = 128;
	resources.maxTessEvaluationTextureImageUnits = 16;
	resources.maxTessEvaluationUniformComponents = 1024;
	resources.maxTessPatchComponents = 120;
	resources.maxPatchVertices = 32;
	resources.maxTessGenLevel = 64;
	resources.maxViewports = 16;
	resources.maxVertexAtomicCounters = 0;
	resources.maxTessControlAtomicCounters = 0;
	resources.maxTessEvaluationAtomicCounters = 0;
	resources.maxGeometryAtomicCounters = 0;
	resources.maxFragmentAtomicCounters = 8;
	resources.maxCombinedAtomicCounters = 8;
	resources.maxAtomicCounterBindings = 1;
	resources.maxVertexAtomicCounterBuffers = 0;
	resources.maxTessControlAtomicCounterBuffers = 0;
	resources.maxTessEvaluationAtomicCounterBuffers = 0;
	resources.maxGeometryAtomicCounterBuffers = 0;
	resources.maxFragmentAtomicCounterBuffers = 1;
	resources.maxCombinedAtomicCounterBuffers = 1;
	resources.maxAtomicCounterBufferSize = 16384;
	resources.maxTransformFeedbackBuffers = 4;
	resources.maxTransformFeedbackInterleavedComponents = 64;
	resources.maxCullDistances = 8;
	resources.maxCombinedClipAndCullDistances = 8;
	resources.maxSamples = 4;
	resources.limits.nonInductiveForLoops = true;
	resources.limits.whileLoops = true;
	resources.limits.doWhileLoops = true;
	resources.limits.generalUniformIndexing = true;
	resources.limits.generalAttributeMatrixVectorIndexing = true;
	resources.limits.generalVaryingIndexing = true;
	resources.limits.generalSamplerIndexing = true;
	resources.limits.generalVariableIndexing = true;
	resources.limits.generalConstantMatrixVectorIndexing = true;
	return resources;
}

inline std::vector<uint32_t> gCompileToSpirv(const glslang::TProgram &program, int language) {
	glslang::SpvOptions spvOptions;
#ifdef NODEBUG
	spvOptions.generateDebugInfo = true;
	spvOptions.disableOptimizer = true;
	spvOptions.optimizeSize = false;
#else
	spvOptions.generateDebugInfo = false;
	spvOptions.disableOptimizer = false;
	spvOptions.optimizeSize = true;
#endif

	spv::SpvBuildLogger logger;
	std::vector<uint32_t> spirv;
	GlslangToSpv(*program.getIntermediate(static_cast<EShLanguage>(language)), spirv, &logger, &spvOptions);
	return spirv;
}

class ShaderIncluder : 	public glslang::TShader::Includer {
public:
	IncludeResult *includeLocal(const char *headerName, const char *includerName, size_t inclusionDepth) override {
		auto directory = std::filesystem::path(includerName).parent_path();
		auto fileLoaded = FileManager::ReadFile(directory / headerName);

		if (!fileLoaded) {
			LOG_CRITICAL("Shader Include could not be loaded: {}", headerName);
		}

		auto content = new char[fileLoaded->size()];
		std::memcpy(content, fileLoaded->c_str(), fileLoaded->size());
		return new IncludeResult(headerName, content, fileLoaded->size(), content);
	}

	IncludeResult *includeSystem(const char *headerName, const char *includerName, size_t inclusionDepth) override {
		auto fileLoaded = FileManager::ReadFile(headerName);

		if (!fileLoaded) {
			LOG_CRITICAL("Shader Include could not be loaded: {}", headerName);
		}

		auto content = new char[fileLoaded->size()];
		std::memcpy(content, fileLoaded->c_str(), fileLoaded->size());
		return new IncludeResult(headerName, content, fileLoaded->size(), content);
	}

	void releaseInclude(IncludeResult *result) override {
		if (result) {
			delete[] static_cast<char *>(result->userData);
			delete result;
		}
	}
};


inline std::string packDefineString(const std::vector<Shader::Define> &defines) {
	std::stringstream defineStream;
	for (const auto &[name, value] : defines) {
		defineStream << "#define " << name << " " << value << "\n";
	}
	return defineStream.str();
}

inline VkShaderStageFlagBits getShaderStageByFileExt(std::string_view ext) {
	auto str = std::string(ext);
	std::ranges::transform(str, str.begin(), tolower);

	if (str == ".vert")
		return VK_SHADER_STAGE_VERTEX_BIT;
	if (str == ".frag")
		return VK_SHADER_STAGE_FRAGMENT_BIT;
	if (str == ".comp")
		return VK_SHADER_STAGE_COMPUTE_BIT;
	if (str == ".tesc")
		return VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT;
	if (str == ".tese")
		return VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT;
	if (str == ".geom")
		return VK_SHADER_STAGE_GEOMETRY_BIT;
	return VK_SHADER_STAGE_ALL;
}

inline VkPipelineShaderStageCreateInfo gMakeShaderStageCreateInfo(VkShaderStageFlagBits stage, VkShaderModule module) {
	VkPipelineShaderStageCreateInfo pipelineShaderStageCreateInfo = {};
	pipelineShaderStageCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
	pipelineShaderStageCreateInfo.stage = stage;
	pipelineShaderStageCreateInfo.module = module;
	pipelineShaderStageCreateInfo.pName = "main";
	return pipelineShaderStageCreateInfo;
}

constexpr VkDescriptorType gUniformTypeToVkType[] = {
	VK_DESCRIPTOR_TYPE_MAX_ENUM,
	VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER,
	VK_DESCRIPTOR_TYPE_STORAGE_BUFFER,
};

Shader::Shader(const std::vector<std::filesystem::path> &files, const std::optional<std::vector<Define>> &defines) {
	const auto defineStr = defines ? packDefineString(*defines) : std::string();

	for (const auto &path : files) {
		const auto codeStr = FileManager::ReadFile(path);
		if (!codeStr) {
			LOG_CRITICAL("An error occurred when reading the Shader file.");
		}

		const auto stageFlag = getShaderStageByFileExt(path.extension().string());
		compileAndReflect(*codeStr, defineStr, stageFlag);
		m_vecShaderStages.emplace_back(gMakeShaderStageCreateInfo(stageFlag, m_vecShaderModules.back()));
	}
	createVertexInputState();
	createPushConstantRanges();
	createDescriptorSetLayoutBindings();
}

std::optional<uint32_t> Shader::GetDescriptorLocation(const std::string &name) const {
	if (auto it = m_mapDescriptorNameToLocation.find(name); it != m_mapDescriptorNameToLocation.end())
		return it->second;
	return std::nullopt;
}

std::optional<VkDescriptorType> Shader::GetDescriptorType(const std::string &name) const {
    if (auto it = m_mapNameToDescriptorType.find(name); it != m_mapNameToDescriptorType.end())
        return it->second;
    return std::nullopt;
}

std::optional<Shader::UniformBlock> Shader::GetUniformBlock(std::string_view name) const {
    if (auto it = m_mapUniformBlock.find(name.data()); it != m_mapUniformBlock.end())
        return it->second;
    return std::nullopt;
}

std::optional<Shader::Uniform> Shader::GetUniform(std::string_view name) const {
    if (auto it = m_mapUniform.find(name.data()); it != m_mapUniform.end())
        return it->second;
    return std::nullopt;
}

VkFormat Shader::GlTypeToVk(int32_t type) {
	switch (type) {
		case 0x1406: // GL_FLOAT
			return VK_FORMAT_R32_SFLOAT;
		case 0x8B50: // GL_FLOAT_VEC2
			return VK_FORMAT_R32G32_SFLOAT;
		case 0x8B51: // GL_FLOAT_VEC3
			return VK_FORMAT_R32G32B32_SFLOAT;
		case 0x8B52: // GL_FLOAT_VEC4
			return VK_FORMAT_R32G32B32A32_SFLOAT;
		case 0x1404: // GL_INT
			return VK_FORMAT_R32_SINT;
		case 0x8B53: // GL_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8B54: // GL_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8B55: // GL_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		case 0x1405: // GL_UNSIGNED_INT
			return VK_FORMAT_R32_SINT;
		case 0x8DC6: // GL_UNSIGNED_INT_VEC2
			return VK_FORMAT_R32G32_SINT;
		case 0x8DC7: // GL_UNSIGNED_INT_VEC3
			return VK_FORMAT_R32G32B32_SINT;
		case 0x8DC8: // GL_UNSIGNED_INT_VEC4
			return VK_FORMAT_R32G32B32A32_SINT;
		default:
			return VK_FORMAT_UNDEFINED;
	}
}

void Shader::IncrementDescriptorPool(std::map<VkDescriptorType, uint32_t> &descriptorPoolCounts, VkDescriptorType type) {
	if (type == VK_DESCRIPTOR_TYPE_MAX_ENUM)
		return;

	if (auto it = descriptorPoolCounts.find(type); it != descriptorPoolCounts.end())
		it->second++;
	else
		descriptorPoolCounts.emplace(type, 1);
}

void Shader::loadUniformBlock(const glslang::TProgram &program, VkShaderStageFlags stageFlag, int32_t i) {
	auto reflection = program.getUniformBlock(i);
    const auto binding = reflection.getBinding();

	for (auto &[uniformBlockName, uniformBlock] : m_mapUniformBlock) {
		if (uniformBlockName == reflection.name) {
			uniformBlock.stageFlags |= stageFlag;
			return;
		}
	}

	auto type = UniformBlock::Type::None;
	if (reflection.getType()->getQualifier().storage == glslang::EvqUniform)
		type = UniformBlock::Type::Uniform;
	if (reflection.getType()->getQualifier().storage == glslang::EvqBuffer)
		type = UniformBlock::Type::Storage;
	if (reflection.getType()->getQualifier().layoutPushConstant)
		type = UniformBlock::Type::PushConstant;

	m_mapUniformBlock.emplace(reflection.name, UniformBlock(reflection.getBinding(), reflection.size, stageFlag, type));
}

/**
 * @brief 从编译后的着色器程序加载 Uniform 变量信息
 * @param program 已编译并完成反射的 glslang 程序
 * @param stageFlag 当前处理的着色器阶段标志 (如 VK_SHADER_STAGE_VERTEX_BIT)
 * @param i Uniform 变量在反射列表中的索引
 *
 * 处理两种情况：
 * 1. 属于 UniformBlock 的成员变量（名称含 "."，如 "Global.time"）
 * 2. 独立的全局 Uniform 变量
 *
 * 若变量已存在，则合并其 stageFlags（支持多阶段着色器）；否则插入新条目。
 */
void Shader::loadUniform(const glslang::TProgram &program, VkShaderStageFlags stageFlag, int32_t i) {
	auto reflection = program.getUniform(i);
    const auto binding = reflection.getBinding();

	if (binding == -1) {
		auto splitName = String::Split(reflection.name, '.');

		if (splitName.size() > 1) {
			for (auto &[uniformBlockName, uniformBlock] : m_mapUniformBlock) {
				if (uniformBlockName == splitName.at(0)) {
					auto key = String::ReplaceFirst(reflection.name, splitName.at(0) + ".", "");
					uniformBlock[key] = {
						binding,
						reflection.offset,
						ComputeSize(reflection.getType()),
						reflection.glDefineType,
						false,
						false,
						stageFlag
					};
					return;
				}
			}
		}
	}

	for (auto &[uniformName, uniform] : m_mapUniform) {
		if (uniformName == reflection.name) {
			uniform.stageFlags |= stageFlag;
			return;
		}
	}

	auto &qualifier = reflection.getType()->getQualifier();
	m_mapUniform.emplace(reflection.name, Uniform(binding, reflection.offset, -1, reflection.glDefineType, qualifier.readonly, qualifier.writeonly, stageFlag));
}

void Shader::loadAttribute(const glslang::TProgram &program, VkShaderStageFlags stageFlag, int32_t i) {
	auto reflection = program.getPipeInput(i);

	if (reflection.name.empty())
		return;

	for (const auto &attribute: m_attributes | std::views::values) {
		if (attribute.name == reflection.name)
			return;
	}

	auto &qualifier = reflection.getType()->getQualifier();
	m_attributes.emplace(qualifier.layoutLocation, Attribute(reflection.name, qualifier.layoutSet, qualifier.layoutLocation, ComputeSize(reflection.getType()), reflection.glDefineType));
}

int32_t Shader::ComputeSize(const glslang::TType *ttype) {
	// TODO: glslang::TType::computeNumComponents is available but has many issues resolved in this method.
	int32_t components = 0;

	if (ttype->getBasicType() == glslang::EbtStruct || ttype->getBasicType() == glslang::EbtBlock) {
		for (const auto &tl : *ttype->getStruct())
			components += ComputeSize(tl.type);
	} else if (ttype->getMatrixCols() != 0) {
		components = ttype->getMatrixCols() * ttype->getMatrixRows();
	} else {
		components = ttype->getVectorSize();
	}

	if (ttype->getArraySizes()) {
		int32_t arraySize = 1;

		for (int32_t d = 0; d < ttype->getArraySizes()->getNumDims(); ++d) {
			// This only makes sense in paths that have a known array size.
			if (auto dimSize = ttype->getArraySizes()->getDimSize(d); dimSize != glslang::UnsizedArraySize)
				arraySize *= dimSize;
		}

		components *= arraySize;
	}

	return sizeof(float) * components;
}

void Shader::compileAndReflect(std::string_view code, std::string_view defines, VkShaderStageFlags moduleFlag) {
	// Starts converting GLSL to SPIR-V.
	auto language = GetEshLanguage(moduleFlag);
	glslang::TProgram program;
	glslang::TShader shader(language);
	constexpr auto resources = GetResources();

	// Enable SPIR-V and Vulkan rules when parsing GLSL.
	auto messages = static_cast<EShMessages>(EShMsgSpvRules | EShMsgVulkanRules | EShMsgDefault);
#ifndef NODEBUG
	messages = static_cast<EShMessages>(messages | EShMsgDebugInfo);
#endif

	constexpr auto defaultVersion = glslang::EShTargetVulkan_1_4;
	auto shaderSource = code.data();
	shader.setStringsWithLengthsAndNames(&shaderSource, nullptr, nullptr, 1);
	shader.setPreamble(defines.data());
	shader.setEnvInput(glslang::EShSourceGlsl, language, glslang::EShClientVulkan, 110);
	shader.setEnvClient(glslang::EShClientVulkan, defaultVersion);
	shader.setEnvTarget(glslang::EShTargetSpv, volkGetInstanceVersion() >= VK_API_VERSION_1_1 ? glslang::EShTargetSpv_1_3 : glslang::EShTargetSpv_1_0);

	ShaderIncluder includer;

	std::string str;
	if (!shader.preprocess(&resources, defaultVersion, ENoProfile, false, false, messages, &str, includer)) {
		LOG_INFO("{}", shader.getInfoLog());
		LOG_INFO("{}", shader.getInfoDebugLog());
		LOG_CRITICAL("SPRIV shader preprocess failed!");
	}

	if (!shader.parse(&resources, defaultVersion, true, messages, includer)) {
		LOG_INFO("{}", shader.getInfoLog());
		LOG_INFO("{}", shader.getInfoDebugLog());
		LOG_CRITICAL("SPRIV shader parse failed!\n");
	}

	program.addShader(&shader);

	if (!program.link(messages) || !program.mapIO()) {
		LOG_CRITICAL("Error while linking shader program.");
	}

	program.buildReflection();
	//program.dumpReflection();

	for (uint32_t dim = 0; dim < 3; ++dim) {
		if (auto localSize = program.getLocalSize(dim); localSize > 1)
			m_localSizes[dim] = localSize;
	}

	for (int32_t i = program.getNumLiveUniformBlocks() - 1; i >= 0; i--)
		loadUniformBlock(program, moduleFlag, i);

	for (int32_t i = 0; i < program.getNumLiveUniformVariables(); i++)
		loadUniform(program, moduleFlag, i);

	for (int32_t i = 0; i < program.getNumLiveAttributes(); i++)
		loadAttribute(program, moduleFlag, i);

	auto spirv = gCompileToSpirv(program, language);
	createAndPushShaderModule(spirv);
}

inline void Shader::createAndPushShaderModule(const std::vector<uint32_t> &spirv) {
	auto logicalDevice = Graphics::Get()->GetLogicDevice();

	VkShaderModule module;
	VkShaderModuleCreateInfo shaderModuleCreateInfo = {};
	shaderModuleCreateInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
	shaderModuleCreateInfo.codeSize = spirv.size() * sizeof(uint32_t);
	shaderModuleCreateInfo.pCode = spirv.data();

	CALL_VK(vkCreateShaderModule(logicalDevice->GetHandle(), &shaderModuleCreateInfo, nullptr, &module));
	m_vecShaderModules.push_back(module);
}

void Shader::createDescriptorSetLayoutBindings() {
	for (const auto &[name, block]: m_mapUniformBlock) {
		auto type = gUniformTypeToVkType[TO_U32(block.type)];
		m_vecDescriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding {
			.binding = TO_U32(block.binding),
			.descriptorType = type,
			.descriptorCount = 1,
			.stageFlags = block.stageFlags
		});
		m_mapNameToDescriptorType[name] = type;
        m_mapDescriptorNameToLocation[name] = block.binding;
	}

	for (const auto &[name, uniform]: m_mapUniform) {
		auto type = uniform.writeOnly ? VK_DESCRIPTOR_TYPE_STORAGE_IMAGE : VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
		m_vecDescriptorSetLayoutBinding.emplace_back(VkDescriptorSetLayoutBinding {
			.binding = TO_U32(uniform.binding),
			.descriptorType = type,
			.descriptorCount = 1,
			.stageFlags = uniform.stageFlags
		});
		m_mapNameToDescriptorType[name] = type;
        m_mapDescriptorNameToLocation[name] = uniform.binding;
	}

	std::sort(m_vecDescriptorSetLayoutBinding.begin(), m_vecDescriptorSetLayoutBinding.end(), [](const auto &lhs, const auto &rhs) {
		return lhs.binding < rhs.binding;
	});
}

void Shader::createVertexInputState() {
	std::vector<VkVertexInputAttributeDescription> vertexAttrDescs;
	std::vector<VkVertexInputAttributeDescription> instanceAttrDescs;
	uint32_t vertexOffset = 0;
	uint32_t instanceOffset = 0;

    for (const auto &[location, attr] : m_attributes) {
		VkVertexInputAttributeDescription attrDesc = {};
		attrDesc.location = attr.location;
		attrDesc.format = GlTypeToVk(attr.glType);

		// 以 'i' 开头的是实例化数据
		if (!attr.name.empty() && attr.name[0] == 'i') {
			attrDesc.binding = 1;
			attrDesc.offset = instanceOffset;
			instanceAttrDescs.push_back(attrDesc);
			instanceOffset += attr.size;
		} else {
			attrDesc.binding = 0;
			attrDesc.offset = vertexOffset;
			vertexAttrDescs.push_back(attrDesc);
			vertexOffset += attr.size;
		}
	}

	// 顶点数据绑定
	if (!vertexAttrDescs.empty()) {
		VkVertexInputBindingDescription vertexBindingDesc = {};
		vertexBindingDesc.binding = 0;
		vertexBindingDesc.stride = vertexOffset;
		vertexBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
		m_vertexInputState.vecBindingDescription.push_back(vertexBindingDesc);
	}

	// 实例化数据绑定
	if (!instanceAttrDescs.empty()) {
		VkVertexInputBindingDescription instanceBindingDesc = {};
		instanceBindingDesc.binding = 1;
		instanceBindingDesc.stride = instanceOffset;
		instanceBindingDesc.inputRate = VK_VERTEX_INPUT_RATE_INSTANCE;
		m_vertexInputState.vecBindingDescription.push_back(instanceBindingDesc);
	}

	m_vertexInputState.attributeDescriptions = std::move(vertexAttrDescs);
	m_vertexInputState.attributeDescriptions.insert(
		m_vertexInputState.attributeDescriptions.end(),
		instanceAttrDescs.begin(),
		instanceAttrDescs.end()
	);

	std::ranges::sort(m_vertexInputState.vecBindingDescription,
		[](const VkVertexInputBindingDescription &lhs, const VkVertexInputBindingDescription &rhs) {
			return lhs.binding < rhs.binding;
    });
}

void Shader::createPushConstantRanges() {
	uint32_t currentOffset = 0;

	for (const auto &[name, block] : m_mapUniformBlock) {
		if (block.type != UniformBlock::Type::PushConstant)
			continue;

		VkPushConstantRange pushConstantRange = {};
		pushConstantRange.stageFlags = block.stageFlags;
		pushConstantRange.offset = currentOffset;
		pushConstantRange.size = static_cast<uint32_t>(block.size);
		m_vecPushConstantRanges.push_back(pushConstantRange);
		currentOffset += pushConstantRange.size;
	}
}

USING_ENGINE_NAMESPACE_END
