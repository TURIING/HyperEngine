//
// Created by turiing on 2026/1/16.
//

#include "Image.h"

#include "Sampler.h"
#include "../../Graphics.h"
#include "../../device/LogicDevice.h"
#include "../../device/PhysicalDevice.h"
#include "../../command/CommandBuffer.h"
#include "../ResourceCache.h"

USING_ENGINE_NAMESPACE_BEGIN
Image::Image(const ImageCreateInfo &info) {
    m_pLogicDevice = Graphics::Get()->GetLogicDevice();
    m_aspect = info.aspect;
    m_extent = info.extent;
    m_usage = info.usage;
    m_arrayLayers = info.arrayLayers;
    m_samples = info.samples;
    m_name = info.name;
    m_format = info.format;
    m_viewType = info.viewType;
    m_memoryPropertyFlags = info.memoryPropertyFlags;
    m_mipLevels = info.mipLevels;
    m_imageType = info.imageType;
    m_pSampler = Graphics::Get()->GetResourceCache()->RequestSampler( {
        .magFilter = info.filter,
        .minFilter = info.filter,
        .addressModeU = info.samplerAddressMode,
        .addressModeV = info.samplerAddressMode,
        .addressModeW = info.samplerAddressMode,
    });
    createImage();
    createImageView();
}

Image::Image(VkImage handle, const ImageCreateInfo &info) {
    m_pLogicDevice = Graphics::Get()->GetLogicDevice();
    m_aspect = info.aspect;
    m_extent = info.extent;
    m_usage = info.usage;
    m_arrayLayers = info.arrayLayers;
    m_samples = info.samples;
    m_name = info.name;
    m_format = info.format;
    m_viewType = info.viewType;
    m_memoryPropertyFlags = info.memoryPropertyFlags;
    m_mipLevels = info.mipLevels;
    m_imageType = info.imageType;
    m_pSampler = Graphics::Get()->GetResourceCache()->RequestSampler( {
        .magFilter = info.filter,
        .minFilter = info.filter,
        .addressModeU = info.samplerAddressMode,
        .addressModeV = info.samplerAddressMode,
        .addressModeW = info.samplerAddressMode,
    });
    m_pHandle = handle;
    createImageView();
}

Image::~Image() {
    vkDestroyImage(m_pLogicDevice->GetHandle(), m_pHandle, nullptr);
    vkFreeMemory(m_pLogicDevice->GetHandle(), m_pDeviceMemory, nullptr);
    vkDestroyImageView(m_pLogicDevice->GetHandle(), m_imageView, nullptr);
}

WriteDescriptorSet Image::GetWriteDescriptorSet(uint32_t binding, VkDescriptorType descriptorType) const {
    VkDescriptorImageInfo imageInfo {};
    imageInfo.sampler = m_pSampler->GetHandle();
    imageInfo.imageView = m_imageView;
    imageInfo.imageLayout = m_currentLayout;

    VkWriteDescriptorSet descriptorWrite = {};
    descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
    descriptorWrite.dstSet = VK_NULL_HANDLE; // Will be set in the descriptor handler.
    descriptorWrite.dstBinding = binding;
    descriptorWrite.dstArrayElement = 0;
    descriptorWrite.descriptorCount = 1;
    descriptorWrite.descriptorType = descriptorType;

    return { descriptorWrite, imageInfo };
}

void Image::createImage() {
    // 生成mipmap需要GPU支持对该格式做线性过滤
    if (m_mipLevels > 1) {
        VkFormatProperties formatProperties;
        vkGetPhysicalDeviceFormatProperties(m_pLogicDevice->GetPhysicalDevice()->GetHandle(), m_format, &formatProperties);

        if (!(formatProperties.optimalTilingFeatures & VK_FORMAT_FEATURE_SAMPLED_IMAGE_FILTER_LINEAR_BIT)) {
            LOG_CRITICAL("Texture image format does not support linear blitting!");
        }

        // 如果要生成 mipmap，必须包含 VK_IMAGE_USAGE_TRANSFER_SRC_BIT（因为高层要拷贝给低层）
        if ((m_usage & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) == 0) {
            m_usage |= VK_IMAGE_USAGE_TRANSFER_SRC_BIT;
        }
    }

    const VkImageCreateInfo imageCreateInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = m_imageType,
        .format = m_format,
        .extent = {
            .width = m_extent.width,
            .height = m_extent.height,
            .depth = m_extent.depth,
        },
        .mipLevels = m_mipLevels,
        .arrayLayers = m_arrayLayers,
        .samples = m_samples,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = m_usage,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
    };
    CALL_VK(vkCreateImage(m_pLogicDevice->GetHandle(), &imageCreateInfo, nullptr, &m_pHandle));

    // 分配图像内存
    VkMemoryRequirements memoryRequirements;
    vkGetImageMemoryRequirements(m_pLogicDevice->GetHandle(), m_pHandle, &memoryRequirements);

    VkMemoryAllocateInfo memoryAllocateInfo {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = memoryRequirements.size,
        .memoryTypeIndex = m_pLogicDevice->GetPhysicalDevice()->FindMemoryType(memoryRequirements.memoryTypeBits, m_memoryPropertyFlags),
    };
    CALL_VK(vkAllocateMemory(m_pLogicDevice->GetHandle(), &memoryAllocateInfo, nullptr, &m_pDeviceMemory));

    vkBindImageMemory(m_pLogicDevice->GetHandle(), m_pHandle, m_pDeviceMemory, 0);

    if (!m_name.empty()) {
        m_pLogicDevice->SetDebugUtilsObjectName(VK_OBJECT_TYPE_IMAGE, (uint64_t)m_pHandle, m_name.data());
    }
}

void Image::createImageView() {
    VkImageViewCreateInfo imageViewCreateInfo {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = m_pHandle,
        .viewType = m_viewType,
        .format = m_format,
        .components = {
            .r = VK_COMPONENT_SWIZZLE_IDENTITY,
            .g = VK_COMPONENT_SWIZZLE_IDENTITY,
            .b = VK_COMPONENT_SWIZZLE_IDENTITY,
            .a = VK_COMPONENT_SWIZZLE_IDENTITY,
        },
        .subresourceRange = {
            .aspectMask = m_aspect,
            .baseMipLevel = 0,
            .levelCount = m_mipLevels,
            .baseArrayLayer = 0,
            .layerCount = 1
        },
    };
    CALL_VK(vkCreateImageView(m_pLogicDevice->GetHandle(), &imageViewCreateInfo, nullptr, &m_imageView));
}

USING_ENGINE_NAMESPACE_END
