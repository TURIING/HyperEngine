/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 20:17
* @version: 1.0
* @description: 
********************************************************************************/
#include "Buffer.h"

#include "../../device/LogicDevice.h"
#include "../../device/PhysicalDevice.h"
#include "../../Graphics.h"
USING_ENGINE_NAMESPACE_BEGIN

Buffer::Buffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) : m_size(size) {
	m_pLogicDevice = Graphics::Get()->GetLogicDevice();

	// 创建缓冲
	VkBufferCreateInfo bufferInfo{
		.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO,
		.size = size,
		.usage = usage,
		.sharingMode = VK_SHARING_MODE_EXCLUSIVE
    };
    CALL_VK(vkCreateBuffer(m_pLogicDevice->GetHandle(), &bufferInfo, nullptr, &m_pHandle));

    // 获取缓冲的内存需求
    VkMemoryRequirements memoryRequirements;
    vkGetBufferMemoryRequirements(m_pLogicDevice->GetHandle(), m_pHandle, &memoryRequirements);

    // 分配内存
    const VkMemoryAllocateInfo allocateInfo {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = m_pLogicDevice->GetPhysicalDevice()->FindMemoryType(memoryRequirements.memoryTypeBits, properties)
    };
    CALL_VK(vkAllocateMemory(m_pLogicDevice->GetHandle(), &allocateInfo, nullptr, &m_pDeviceMemory));

    // 将缓冲区绑定到设备内存
    vkBindBufferMemory(m_pLogicDevice->GetHandle(), m_pHandle, m_pDeviceMemory, 0);
}

Buffer::~Buffer() {
    vkDestroyBuffer(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
    vkFreeMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory, nullptr);
}

void Buffer::WriteData(uint64_t offset, uint64_t size, const void* data) const {
    void *tempPtr = nullptr;
    vkMapMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory, offset, size, 0, &tempPtr);
    memcpy(tempPtr, data, size);
    vkUnmapMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory);
}

void Buffer::ReadData(uint64_t offset, uint64_t size, void *pData) const {
	void *tempPtr = nullptr;
	vkMapMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory, offset, size, 0, &tempPtr);
	memcpy(pData, tempPtr, size);
	vkUnmapMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory);
}

void Buffer::Map(void **data, uint64_t offset, uint64_t size) const {
	vkMapMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory, offset, size == 0 ? m_size: size, 0, data);
}

void Buffer::Unmap() const {
	vkUnmapMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory);
}

USING_ENGINE_NAMESPACE_END