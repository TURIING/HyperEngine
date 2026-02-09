/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 15:58
* @version: 1.0
* @description: 
********************************************************************************/
#include "LogicDevice.h"
#include "Instance.h"
#include "PhysicalDevice.h"

#define VMA_IMPLEMENTATION
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#include <vma/include/vk_mem_alloc.h>

USING_ENGINE_NAMESPACE_BEGIN

LogicDevice::LogicDevice(const Share<Instance> &pInstance, const Share<PhysicalDevice> &pPhysicalDevice, const LogicDeviceCreateInfo& info)
	: m_pInstance(pInstance), m_pPhysicalDevice(pPhysicalDevice) {

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
	volkLoadDevice(m_pHandle);
	LOG_DEBUG("LogicDevice Created.");

	// createMemoryAllocator();

	vkGetDeviceQueue(m_pHandle, m_pPhysicalDevice->GetQueueFamily(QueueType::Graphics), 0, &m_graphicsQueue);
	vkGetDeviceQueue(m_pHandle, m_pPhysicalDevice->GetQueueFamily(QueueType::Compute), 0, &m_computeQueue);
	vkGetDeviceQueue(m_pHandle, m_pPhysicalDevice->GetQueueFamily(QueueType::Transfer), 0, &m_transferQueue);
	vkGetDeviceQueue(m_pHandle, m_pPhysicalDevice->GetQueueFamily(QueueType::Present), 0, &m_presentQueue);
}

LogicDevice::~LogicDevice() {
	CALL_VK(vkDeviceWaitIdle(m_pHandle));
	vkDestroyDevice(m_pHandle, nullptr);
}

void LogicDevice::SetDebugUtilsObjectName(VkObjectType type, uint64_t handle, const char *name) const {
	VkDebugUtilsObjectNameInfoEXT nameInfo{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
		.pNext = nullptr,
		.objectType = type,
		.objectHandle = handle,
		.pObjectName = name,
	};
	vkSetDebugUtilsObjectNameEXT(m_pHandle, &nameInfo);
}

void LogicDevice::createMemoryAllocator() {
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

LogicDeviceBuilder LogicDevice::Builder() {
    return LogicDeviceBuilder{ };
}

LogicDeviceBuilder& LogicDeviceBuilder::AddExtension(const char* ext) {
    m_createInfo.extensions.push_back(ext);
    return *this;
}

LogicDeviceBuilder& LogicDeviceBuilder::SetQueueInfos(QueueInfo* pQueueInfo, u32 count) {
	m_createInfo.pQueueInfo = pQueueInfo;
	m_createInfo.queueInfoCount = count;
	return *this;
}

Share<LogicDevice> LogicDeviceBuilder::Build() const {
	return std::make_shared<LogicDevice>(m_pInstance, m_pPhysicalDevice, m_createInfo);
}

USING_ENGINE_NAMESPACE_END