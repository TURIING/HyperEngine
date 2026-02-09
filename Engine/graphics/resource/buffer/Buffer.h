/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/3/10 20:17
* @version: 1.0
* @description: 
********************************************************************************/
#ifndef VULBUFFER_H
#define VULBUFFER_H

#include "../../VulObject.h"
#include "../../../common/Common.h"

USING_ENGINE_NAMESPACE_BEGIN
class LogicDevice;

class Buffer : public VulObject<VkBuffer> {
public:
	enum class Status { Reset, Changed, Normal };
	 Buffer(VkDeviceSize size, VkBufferUsageFlags usage = VK_BUFFER_USAGE_TRANSFER_DST_BIT, VkMemoryPropertyFlags properties = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
	~Buffer() override;
	NODISCARD uint64_t GetSize() const { return m_size; }
	void WriteData(uint64_t offset, uint64_t size, const void* data) const;
	void ReadData(uint64_t offset, uint64_t size, void* pData) const;
	void Map(void **data, uint64_t offset = 0, uint64_t size = 0) const;
	void Unmap() const;

protected:
	Share<LogicDevice> m_pLogicDevice	= nullptr;
	VkDeviceMemory	m_pDeviceMemory = nullptr;
	VkDeviceSize	m_size			= 0;
};

USING_ENGINE_NAMESPACE_END
#endif // VULBUFFER_H
