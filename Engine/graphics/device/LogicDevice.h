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
#include <vma/include/vk_mem_alloc.h>

USING_ENGINE_NAMESPACE_BEGIN
class CommandBuffer;
class LogicDevice;
class Surface;
class PhysicalDevice;
class Instance;

struct LogicDeviceCreateInfo {
    std::vector<const char*> extensions;
    VkPhysicalDeviceFeatures deviceFeatures;
	QueueInfo* pQueueInfo = nullptr;
	u32 queueInfoCount = 0;
};

class LogicDeviceBuilder {
public:
	LogicDeviceBuilder& SetInstance(const Share<Instance> &instance) { m_pInstance = instance; return *this; }
	LogicDeviceBuilder& SetPhysicalDevice(const Share<PhysicalDevice> &physicalDevice) { m_pPhysicalDevice = physicalDevice; return *this; }
	LogicDeviceBuilder& AddExtension(const char* ext);
	LogicDeviceBuilder& SetDeviceFeatures(const VkPhysicalDeviceFeatures& deviceFeatures) { m_createInfo.deviceFeatures = deviceFeatures; return *this; }
	LogicDeviceBuilder& SetQueueInfos(QueueInfo* pQueueInfo, u32 count);
	[[nodiscard]] Share<LogicDevice> Build() const;

private:
	Share<Instance>		  m_pInstance	    = nullptr;
	Share<PhysicalDevice> m_pPhysicalDevice = nullptr;
	LogicDeviceCreateInfo m_createInfo{};
};

class LogicDevice final : public VulObject<VkDevice>{
public:
	 LogicDevice(const Share<Instance> &pInstance, const Share<PhysicalDevice> &pPhysicalDevice, const LogicDeviceCreateInfo& info);
	~LogicDevice() override;
	NODISCARD const Share<PhysicalDevice>& GetPhysicalDevice() const { return m_pPhysicalDevice; }
	NODISCARD const Share<Instance> &GetInstance() const { return m_pInstance; }

	NODISCARD VkQueue GetGraphicsQueue() const { return m_graphicsQueue; }
	NODISCARD VkQueue GetComputeQueue() const { return m_computeQueue; }
	NODISCARD VkQueue GetTransferQueue() const { return m_transferQueue; }
	NODISCARD VkQueue GetPresentQueue() const { return m_presentQueue; }

	void SetDebugUtilsObjectName(VkObjectType type, uint64_t handle, const char* name) const;
	static LogicDeviceBuilder Builder();
private:
	void createMemoryAllocator();

private:
	Share<Instance>	   m_pInstance		 = nullptr;
	Share<PhysicalDevice> m_pPhysicalDevice = nullptr;
    VmaAllocator m_pAllocator = VK_NULL_HANDLE;

	VkQueue m_graphicsQueue	= VK_NULL_HANDLE;
	VkQueue m_computeQueue	= VK_NULL_HANDLE;
	VkQueue m_transferQueue	= VK_NULL_HANDLE;
	VkQueue m_presentQueue	= VK_NULL_HANDLE;
};

USING_ENGINE_NAMESPACE_END
#endif // VULLOGICDEVICE_H
