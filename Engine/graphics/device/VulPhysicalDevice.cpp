/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/23 13:20
* @version: 1.0
* @description: 
********************************************************************************/

#include "VulPhysicalDevice.h"
#include "VulInstance.h"

USING_ENGINE_NAMESPACE_BEGIN

VulPhysicalDevice::VulPhysicalDevice(VulInstance* pInstance, const VulPhysicalDeviceCreateInfo& createInfo) : m_pInstance(pInstance) {
	m_pInstance->AddRef();

	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(m_pInstance->GetHandle(), &deviceCount, nullptr);
	LOG_ASSERT_INFO(deviceCount != 0, "Failed to find GPUs with Vulkan support!");

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(m_pInstance->GetHandle(), &deviceCount, devices.data());
	for(const auto& device : devices) {
		if(this->checkDeviceSupport(device, createInfo.extensions)) {
			m_pHandle = device;
			break;
		}
	}

    // 获取队列族索引
    this->acquireQueueFamilyIndices();

	this->acquireDepthFormatDetail();

	vkGetPhysicalDeviceProperties(m_pHandle, &m_physicalDeviceProperties);

	LOG_ASSERT_INFO(m_pHandle != VK_NULL_HANDLE, "failed to find a suitable GPU!");
}

VulPhysicalDevice::~VulPhysicalDevice() {
	m_pInstance->SubRef();
}

bool VulPhysicalDevice::checkDeviceSupport(VkPhysicalDevice device, const std::vector<const char *>& extensions) {
    const auto extensionSupported = checkDeviceExtensionSupport(device, extensions);

    VkPhysicalDeviceFeatures supportFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportFeatures);

    return extensionSupported && supportFeatures.samplerAnisotropy;
}

bool VulPhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char *>& extensions) {
    uint32_t extensionCount = 0;
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(extensions.begin(), extensions.end());
    for(const auto &extension : availableExtensions) {
        requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
}

uint32_t VulPhysicalDevice::FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const {
    // 查询物理设备可用的内存类型
    VkPhysicalDeviceMemoryProperties memoryProperties;
    vkGetPhysicalDeviceMemoryProperties(m_pHandle, &memoryProperties);

    for(auto i = 0; i < memoryProperties.memoryTypeCount; i++) {
        if((typeFilter & (1 << i)) && (memoryProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    LOG_CRITICAL("Failed to find suitable memory type.");
}

u32 VulPhysicalDevice::GetQueueFamily(QueueType queueType) const {
    std::optional<u32> index;
    switch (queueType) {
    case QueueType::Graphics:   index = sGetQueueFamilyIndex(m_queueFamilyProperties, VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_TRANSFER_BIT);  break;
    case QueueType::Compute:    index = sGetQueueFamilyIndex(m_queueFamilyProperties, VK_QUEUE_COMPUTE_BIT, VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_TRANSFER_BIT);  break;
    case QueueType::Present:    index = sGetQueueFamilyIndex(m_queueFamilyProperties, VK_QUEUE_GRAPHICS_BIT, {});                                                   break;
    case QueueType::Transfer:   index = sGetQueueFamilyIndex(m_queueFamilyProperties, VK_QUEUE_TRANSFER_BIT, VK_QUEUE_GRAPHICS_BIT, VK_QUEUE_COMPUTE_BIT);  break;
    default:                    LOG_ASSERT(false);
    }
    LOG_ASSERT(index);
    return index.value();
}

std::optional<u32> VulPhysicalDevice::sGetQueueFamilyIndex(
    const std::vector<VkQueueFamilyProperties>& queueFamilies,
    VkQueueFlags with,
    VkQueueFlags without1,
    VkQueueFlags without2) {
    if(auto index = sGetQueueFamilyIndex(queueFamilies, with, without1 | without2); index != std::nullopt) {
        return index;
    }

    if(auto index = sGetQueueFamilyIndex(queueFamilies, with, without1); index != std::nullopt) {
        return index;
    }

    if(auto index = sGetQueueFamilyIndex(queueFamilies, with, without2); index != std::nullopt) {
        return index;
    }

    return sGetQueueFamilyIndex(queueFamilies, with, {});
}

std::optional<u32> VulPhysicalDevice::sGetQueueFamilyIndex(const std::vector<VkQueueFamilyProperties>& queueFamilies, VkQueueFlags with, VkQueueFlags without) {
    for(auto i = 0; i < queueFamilies.size(); i++) {
        if((queueFamilies[i].queueFlags & with) == with && !(queueFamilies[i].queueFlags & without)) {
            return i;
        }
    }
    return std::nullopt;
}

void VulPhysicalDevice::acquireDepthFormatDetail() {
    constexpr VkFormat formats[] = { VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT };
    for(const auto format : formats) {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(m_pHandle, format, &formatProperties);

        if(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT) {
            m_depthFormat = format;
            break;
        }
    }
}

void VulPhysicalDevice::acquireQueueFamilyIndices() {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(m_pHandle, &queueFamilyCount, nullptr);

    m_queueFamilyProperties.resize(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(m_pHandle, &queueFamilyCount, m_queueFamilyProperties.data());
}

VulPhysicalDevice* VulPhysicalDeviceBuilder::Build() const {
    return new VulPhysicalDevice(m_pInstance, m_createInfo);
}

VulPhysicalDeviceBuilder& VulPhysicalDeviceBuilder::AddExtension(const char* ext) {
    m_createInfo.extensions.push_back(ext);
    return *this;
}

USING_ENGINE_NAMESPACE_END