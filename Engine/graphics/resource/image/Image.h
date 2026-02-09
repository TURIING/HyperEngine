//
// Created by turiing on 2026/1/16.
//

#ifndef HYPERENGINE_IMAGE_H
#define HYPERENGINE_IMAGE_H
#include "../../../common/Common.h"
#include "../../VulObject.h"
#include "../descriptor/Descriptor.h"

USING_ENGINE_NAMESPACE_BEGIN
class LogicDevice;
class Sampler;

struct ImageCreateInfo {
    VkImageType imageType = VK_IMAGE_TYPE_2D;
    VkExtent3D extent{};
    VkSampleCountFlagBits samples = VK_SAMPLE_COUNT_1_BIT;
    VkImageUsageFlags usage = VK_IMAGE_USAGE_TRANSFER_SRC_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT;
    VkFormat format = VK_FORMAT_R8G8B8A8_UNORM;
    uint32_t mipLevels = 1;
    uint32_t arrayLayers = 1;
    VkMemoryPropertyFlags memoryPropertyFlags = VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT;
    VkImageViewType viewType = VK_IMAGE_VIEW_TYPE_2D;
    VkImageAspectFlags aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    Filter filter = Filter::LINEAR;
    AddressMode samplerAddressMode = AddressMode::REPEAT;
    std::string_view name;
};

class Image: public VulObject<VkImage>, public Descriptor {
public:
    Image(const ImageCreateInfo &info);
    Image(VkImage handle, const ImageCreateInfo &info);
    ~Image() override;
    NODISCARD VkImageLayout GetCurrentImageLayout() const { return m_currentLayout; }
	NODISCARD VkImageAspectFlags GetAspectFlags() const { return m_aspect; }
    NODISCARD VkImageUsageFlags GetUsage() const { return m_usage; }
    NODISCARD VkImageView GetImageView() const { return m_imageView; }
    void SetCurrentImageLayout(VkImageLayout layout) { m_currentLayout = layout; }

    NODISCARD WriteDescriptorSet GetWriteDescriptorSet(uint32_t binding, VkDescriptorType descriptorType, const std::optional<OffsetSize> &offsetSize) const override;

protected:
    void createImage();
    void createImageView();

protected:
    Share<LogicDevice> m_pLogicDevice;
    VkImageType m_imageType;
    VkExtent3D m_extent{};
    VkSampleCountFlagBits m_samples;
    VkMemoryPropertyFlags m_memoryPropertyFlags;
    VkImageUsageFlags m_usage;
    VkFormat m_format = VK_FORMAT_UNDEFINED;
    uint32_t m_mipLevels = 0;
    uint32_t m_arrayLayers;
    VkDeviceMemory m_pDeviceMemory = VK_NULL_HANDLE;
    VkImageView m_imageView = VK_NULL_HANDLE;
    VkImageViewType m_viewType;
    VkImageAspectFlags m_aspect = VK_IMAGE_ASPECT_COLOR_BIT;
    VkImageLayout m_currentLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    std::string_view m_name;
    Share<Sampler> m_pSampler;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_IMAGE_H