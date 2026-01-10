/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/1 15:58
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULLOGICDEVICE_H
#define VULLOGICDEVICE_H

#include "../VulObject.h"
#include "../../common/Common.h"
#define VMA_STATIC_VULKAN_FUNCTIONS 0
#define VMA_DYNAMIC_VULKAN_FUNCTIONS 0
#include "vma/include/vk_mem_alloc.h"

USING_ENGINE_NAMESPACE_BEGIN
class VulCommandBuffer;
class VulLogicDevice;
class VulSurface;
class VulPhysicalDevice;
class VulInstance;

struct VulLogicDeviceCreateInfo {
    std::vector<const char*> extensions;
    VkPhysicalDeviceFeatures deviceFeatures;
	QueueInfo* pQueueInfo = nullptr;
	u32 queueInfoCount = 0;
};

class VulLogicDeviceBuilder {
public:
	VulLogicDeviceBuilder& SetInstance(VulInstance* instance) { m_pInstance = instance; return *this; }
	VulLogicDeviceBuilder& SetPhysicalDevice(VulPhysicalDevice* physicalDevice) { m_pPhysicalDevice = physicalDevice; return *this; }
	VulLogicDeviceBuilder& AddExtension(const char* ext);
	VulLogicDeviceBuilder& SetDeviceFeatures(const VkPhysicalDeviceFeatures& deviceFeatures) { m_createInfo.deviceFeatures = deviceFeatures; return *this; }
	VulLogicDeviceBuilder& SetQueueInfos(QueueInfo* pQueueInfo, u32 count);
	[[nodiscard]] VulLogicDevice* Build() const;

private:
	VulInstance*			 m_pInstance	   = nullptr;
	VulPhysicalDevice*		 m_pPhysicalDevice = nullptr;
	VulLogicDeviceCreateInfo m_createInfo{};
};

class VulLogicDevice final : public VulObject<VkDevice>{
public:
	 VulLogicDevice(VulInstance* pInstance, VulPhysicalDevice* pPhysicalDevice, const VulLogicDeviceCreateInfo& info);
	~VulLogicDevice() override;
	NODISCARD VulPhysicalDevice* GetPhysicalDevice() const { return m_pPhysicalDevice; }
	NODISCARD VulInstance* GetInstance() const { return m_pInstance; }
	// void SetCmdManager(VulkanCmdManager* pCmdManager);
	void WithSingleCmdBuffer(const std::function<void(VulCommandBuffer* cmd)>& func) const;
	void SetDebugUtilsObjectName(VkObjectType type, uint64_t handle, const char* name) const;
	static VulLogicDeviceBuilder Builder();
private:
	void createMemoryAllocator();

private:
	VulInstance*	   m_pInstance		 = nullptr;
	VulPhysicalDevice* m_pPhysicalDevice = nullptr;
	// VulkanCmdManager*  m_pCmdManager	 = nullptr;
    VmaAllocator m_pAllocator = VK_NULL_HANDLE;
};

USING_ENGINE_NAMESPACE_END
#endif // VULLOGICDEVICE_H
