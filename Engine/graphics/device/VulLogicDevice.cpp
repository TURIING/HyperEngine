/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 15:58
* @version: 1.0
* @description: 
********************************************************************************/
#include "VulLogicDevice.h"
#include "VulInstance.h"
#include "VulPhysicalDevice.h"

USING_ENGINE_NAMESPACE_BEGIN

VulLogicDevice::VulLogicDevice(VulInstance* pInstance, VulPhysicalDevice* pPhysicalDevice, const VulLogicDeviceCreateInfo& info)
	: m_pInstance(pInstance), m_pPhysicalDevice(pPhysicalDevice) {
	m_pInstance->AddRef();
	m_pPhysicalDevice->AddRef();

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
	queueCreateInfos.reserve(info.queueInfoCount);

	for(auto i = 0; i < info.queueInfoCount; i++) {
		auto queueFamilyIndex = m_pPhysicalDevice->GetQueueFamily(info.pQueueInfo[i].type);
		auto it = std::find_if(queueCreateInfos.begin(), queueCreateInfos.end(),  [queueFamilyIndex](const VkDeviceQueueCreateInfo& info) {
			if (info.queueFamilyIndex != queueFamilyIndex) {
				return false;
			}
			else {
				return true;
			}
		});
		if (it != queueCreateInfos.end()) continue;

		VkDeviceQueueCreateInfo queueCreateInfo{
			.sType			  = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			.pNext			  = nullptr,
			.queueFamilyIndex = queueFamilyIndex,
			.queueCount		  = 1,
			.pQueuePriorities = &info.pQueueInfo[i].priority,
		};
		queueCreateInfos.push_back(queueCreateInfo);
	}

	VkDeviceCreateInfo createInfo{
		.sType					 = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext					 = nullptr,
		.queueCreateInfoCount	 = static_cast<uint32_t>(queueCreateInfos.size()),
		.pQueueCreateInfos		 = queueCreateInfos.data(),
		.enabledExtensionCount	 = static_cast<uint32_t>(info.extensions.size()),
		.ppEnabledExtensionNames = info.extensions.data(),
		.pEnabledFeatures		 = &info.deviceFeatures,
	};
	if(m_pInstance->IsEnableValidationLayers()) {
		createInfo.enabledLayerCount   = 1;
		createInfo.ppEnabledLayerNames = &VK_LAYER_KHRONOS_VALIDATION;
	}
	CALL_VK(vkCreateDevice(m_pPhysicalDevice->GetHandle(), &createInfo, nullptr, &m_pHandle));
	LOG_INFO("Logical Device created!");

	createMemoryAllocator();
}

VulLogicDevice::~VulLogicDevice() {
	vkDestroyDevice(m_pHandle, nullptr);
	m_pPhysicalDevice->SubRef();
	m_pInstance->SubRef();
}

void VulLogicDevice::WithSingleCmdBuffer(const std::function<void(VulCommandBuffer* cmd)>& func) const {
	// m_pCmdManager->WithSingleCmdBuffer(func);
}

void VulLogicDevice::SetDebugUtilsObjectName(VkObjectType type, uint64_t handle, const char *name) const {
	VkDebugUtilsObjectNameInfoEXT nameInfo{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
		.pNext = nullptr,
		.objectType = type,
		.objectHandle = handle,
		.pObjectName = name,
	};
	vkSetDebugUtilsObjectNameEXT(m_pHandle, &nameInfo);
}

void VulLogicDevice::createMemoryAllocator() {
    VmaVulkanFunctions vulkanFunctions{};
    vulkanFunctions.vkGetPhysicalDeviceProperties = vkGetPhysicalDeviceProperties;
    vulkanFunctions.vkGetPhysicalDeviceMemoryProperties = vkGetPhysicalDeviceMemoryProperties;
    vulkanFunctions.vkAllocateMemory = vkAllocateMemory;
    vulkanFunctions.vkFreeMemory = vkFreeMemory;
    vulkanFunctions.vkMapMemory = vkMapMemory;
    vulkanFunctions.vkUnmapMemory = vkUnmapMemory;
    vulkanFunctions.vkFlushMappedMemoryRanges = vkFlushMappedMemoryRanges;
    vulkanFunctions.vkInvalidateMappedMemoryRanges = vkInvalidateMappedMemoryRanges;
    vulkanFunctions.vkBindBufferMemory = vkBindBufferMemory;
    vulkanFunctions.vkBindImageMemory = vkBindImageMemory;
    vulkanFunctions.vkGetBufferMemoryRequirements = vkGetBufferMemoryRequirements;
    vulkanFunctions.vkGetImageMemoryRequirements = vkGetImageMemoryRequirements;
    vulkanFunctions.vkCreateBuffer = vkCreateBuffer;
    vulkanFunctions.vkDestroyBuffer = vkDestroyBuffer;
    vulkanFunctions.vkCreateImage = vkCreateImage;
    vulkanFunctions.vkDestroyImage = vkDestroyImage;
    vulkanFunctions.vkCmdCopyBuffer = vkCmdCopyBuffer;
    vulkanFunctions.vkGetBufferMemoryRequirements2KHR = vkGetBufferMemoryRequirements2KHR;
    vulkanFunctions.vkGetImageMemoryRequirements2KHR = vkGetImageMemoryRequirements2KHR;
    vulkanFunctions.vkBindBufferMemory2KHR = vkBindBufferMemory2KHR;
    vulkanFunctions.vkBindImageMemory2KHR = vkBindImageMemory2KHR;
    vulkanFunctions.vkGetPhysicalDeviceMemoryProperties2KHR =  vkGetPhysicalDeviceMemoryProperties2KHR;

    VmaAllocatorCreateInfo allocatorCreateInfo = {};
    allocatorCreateInfo.vulkanApiVersion = VK_API_VERSION_1_4;
    allocatorCreateInfo.physicalDevice = m_pPhysicalDevice->GetHandle();
    allocatorCreateInfo.device = m_pHandle;
    allocatorCreateInfo.instance = m_pInstance->GetHandle();
    allocatorCreateInfo.pVulkanFunctions = &vulkanFunctions;
    vmaCreateAllocator(&allocatorCreateInfo, &m_pAllocator);
}

VulLogicDeviceBuilder VulLogicDevice::Builder() {
    return VulLogicDeviceBuilder{ };
}

VulLogicDeviceBuilder& VulLogicDeviceBuilder::AddExtension(const char* ext) {
    m_createInfo.extensions.push_back(ext);
    return *this;
}

VulLogicDeviceBuilder& VulLogicDeviceBuilder::SetQueueInfos(QueueInfo* pQueueInfo, u32 count) {
	m_createInfo.pQueueInfo = pQueueInfo;
	m_createInfo.queueInfoCount = count;
	return *this;
}

VulLogicDevice* VulLogicDeviceBuilder::Build() const {
	return new VulLogicDevice(m_pInstance, m_pPhysicalDevice, m_createInfo);
}

USING_ENGINE_NAMESPACE_END