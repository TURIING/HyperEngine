/********************************************************************************
 * @author: TURIING
 * @email: turiing@163.com
 * @date: 2025/2/23 13:12
 * @version: 1.0
 * @description:
 ********************************************************************************/
#ifndef VULPHYSICALDEVICE_H
#define VULPHYSICALDEVICE_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN

class VulPhysicalDevice;
class VulInstance;
class VulSurface;

struct VulPhysicalDeviceCreateInfo {
	std::vector<const char*> extensions;
};

class VulPhysicalDeviceBuilder {
public:
	VulPhysicalDeviceBuilder& SetVulInstance(VulInstance* pInstance) {
		m_pInstance = pInstance;
		return *this;
	}

	VulPhysicalDeviceBuilder& AddExtension(const char* ext);
	[[nodiscard]] VulPhysicalDevice* Build() const;

private:
	VulInstance*				m_pInstance = nullptr;
	VulPhysicalDeviceCreateInfo m_createInfo;
};

class VulPhysicalDevice final : public VulObject<VkPhysicalDevice> {
public:
	VulPhysicalDevice(VulInstance* pInstance, const VulPhysicalDeviceCreateInfo& createInfo);
	~VulPhysicalDevice() override;
	bool checkDeviceSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	static bool checkDeviceExtensionSupport(VkPhysicalDevice device, const std::vector<const char*>& extensions);
	NODISCARD VkFormat GetDepthFormat() const { return m_depthFormat; }
	NODISCARD uint32_t FindMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties) const;
	NODISCARD VkPhysicalDeviceProperties GetGpuInfo() const { return m_physicalDeviceProperties; }
	NODISCARD u32 GetQueueFamily(QueueType queueType) const;
	static VulPhysicalDeviceBuilder Builder() { return VulPhysicalDeviceBuilder{}; }

private:
	static std::optional<u32> sGetQueueFamilyIndex(const std::vector<VkQueueFamilyProperties>& queueFamilies, VkQueueFlags with, VkQueueFlags without);
	static std::optional<u32> sGetQueueFamilyIndex(const std::vector<VkQueueFamilyProperties>& queueFamilies, VkQueueFlags with, VkQueueFlags without1, VkQueueFlags without2);
	void acquireDepthFormatDetail();
	void acquireQueueFamilyIndices();

private:
	VulInstance*			   m_pInstance = nullptr;
	VkFormat				   m_depthFormat = VK_FORMAT_UNDEFINED;
	VkPhysicalDeviceProperties m_physicalDeviceProperties{};
	std::vector<VkQueueFamilyProperties> m_queueFamilyProperties;
};

USING_ENGINE_NAMESPACE_END
#endif // VULPHYSICALDEVICE_H