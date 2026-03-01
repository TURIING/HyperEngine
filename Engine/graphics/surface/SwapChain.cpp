//
// Created by turiing on 2026/1/15.
//

#include "SwapChain.h"
#include "Surface.h"
#include "../Graphics.h"
#include "../device/LogicDevice.h"
#include "../device/PhysicalDevice.h"
#include "../resource/image/Image2D.h"
#include "../sync/Semaphore.h"
#include "graphics/command/CmdManager.h"
#include "../command/CommandBuffer.h"

USING_ENGINE_NAMESPACE_BEGIN
    static const std::vector<VkCompositeAlphaFlagBitsKHR> COMPOSITE_ALPHA_FLAGS = {
	VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR, VK_COMPOSITE_ALPHA_PRE_MULTIPLIED_BIT_KHR,
	VK_COMPOSITE_ALPHA_POST_MULTIPLIED_BIT_KHR, VK_COMPOSITE_ALPHA_INHERIT_BIT_KHR,
};

SwapChain::SwapChain(const Share<Surface> &pSurface, const Share<SwapChain> &pOldSwapChain): m_pSurface(pSurface) {
	m_pLogicalDevice = Graphics::Get()->GetLogicDevice();
	m_pPhysicalDevice = Graphics::Get()->GetPhysicalDevice();

	auto graphicsFamily = m_pPhysicalDevice->GetQueueFamily(QueueType::Graphics);
	auto presentFamily = m_pPhysicalDevice->GetQueueFamily(QueueType::Present);

	acquireSwapChainSupportDetails();
	chooseSwapSurfaceFormat();
	VkPresentModeKHR presentMode = this->chooseSwapPresentMode();
	VkExtent2D swapExtent = this->getSwapChainExtent();
	uint32_t imageCount = this->getSwapChainImageCount();

	VkSwapchainCreateInfoKHR swapChainCreateInfo {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = m_pSurface->GetHandle(),
		.minImageCount = imageCount,
		.imageFormat = m_pSwapChainSurfaceFormat.format,
		.imageColorSpace = m_pSwapChainSurfaceFormat.colorSpace,
		.imageExtent = swapExtent,
		.imageArrayLayers = 1,                                                                  // 指定每个图像所包含的层次。通常，来说它的值为1, 但对于VR相关的应用程序来说，会使用更多的层次
		.imageUsage = VK_IMAGE_USAGE_TRANSFER_DST_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,	// 我们在图像上进行绘制操作，也就是将图像作为一个颜色附着来使用
		.oldSwapchain = pOldSwapChain ? pOldSwapChain->GetHandle() : nullptr,
	};
	uint32_t queueFamilyIndices[] = {graphicsFamily, presentFamily};
	if (graphicsFamily != presentFamily) {
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		swapChainCreateInfo.queueFamilyIndexCount = 2;
		swapChainCreateInfo.pQueueFamilyIndices = queueFamilyIndices;
	}
	else {
		swapChainCreateInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
	}

	swapChainCreateInfo.preTransform = m_swapChainSupportDetails.capabilities.currentTransform;
	// 为交换链中的图像指定一个固定的变换操作,比如顺时针旋转90度或是水平翻转
	swapChainCreateInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;                             // 用于指定alpha通道是否被用来和窗口系统中的其它窗口进行混合操作。
	swapChainCreateInfo.presentMode = presentMode;
	swapChainCreateInfo.clipped = VK_TRUE;                                                              // 设置为VK TRUE表示我们不关心被窗口系统中的其它窗口遮挡的像素的颜色
	swapChainCreateInfo.oldSwapchain = VK_NULL_HANDLE;
	CALL_VK(vkCreateSwapchainKHR(m_pLogicalDevice->GetHandle(), &swapChainCreateInfo, nullptr, &m_pHandle));
	LOG_DEBUG("Created SwapChain");

    createSwapChainImages();
}

SwapChain::~SwapChain() {
	vkDestroySwapchainKHR(m_pLogicalDevice->GetHandle(), m_pHandle, nullptr);
}

VkResult SwapChain::AcquireNextImage(Semaphore* pSemaphore, uint32_t &imageIndex) const {
    return vkAcquireNextImageKHR(m_pLogicalDevice->GetHandle(), m_pHandle, UINT64_MAX, pSemaphore->GetHandle(), nullptr, &imageIndex);
}

void SwapChain::acquireSwapChainSupportDetails() {
	auto surface = m_pSurface->GetHandle();
	auto physicalDevice = m_pPhysicalDevice->GetHandle();

	CALL_VK(vkGetPhysicalDeviceSurfaceCapabilitiesKHR(physicalDevice, surface, &m_swapChainSupportDetails.capabilities));

	uint32_t formatCount = 0;
	CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, nullptr));
	if (formatCount != 0) {
		m_swapChainSupportDetails.formats.resize(formatCount);
		CALL_VK(vkGetPhysicalDeviceSurfaceFormatsKHR(physicalDevice, surface, &formatCount, m_swapChainSupportDetails.formats.data()));
	}

	uint32_t presentModeCount = 0;
	CALL_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, nullptr));
	if (presentModeCount != 0) {
		m_swapChainSupportDetails.presentModes.resize(presentModeCount);
		CALL_VK(vkGetPhysicalDeviceSurfacePresentModesKHR(physicalDevice, surface, &presentModeCount, m_swapChainSupportDetails.presentModes.data()));
	}

	if (m_swapChainSupportDetails.formats.empty() && m_swapChainSupportDetails.presentModes.empty()) {
		LOG_CRITICAL("swap chain formats not supported");
	}
}

void SwapChain::chooseSwapSurfaceFormat() {
	for (const auto& availableFormat : m_swapChainSupportDetails.formats) {
		// 1.表示我们以B，G，R和A的顺序,每个颜色通道用8位无符号整型数表示，总共每像素使用32位表示
		// 2.表示SRGB颜色空间是否被支持
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace ==
			VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			m_pSwapChainSurfaceFormat = availableFormat;
			return;
			}
	}
	m_pSwapChainSurfaceFormat = m_swapChainSupportDetails.formats[0];
}

VkPresentModeKHR SwapChain::chooseSwapPresentMode() const {
	for (const auto& availablePresentMode : m_swapChainSupportDetails.presentModes) {
		if(availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			return availablePresentMode;
		}
	}
	return VK_PRESENT_MODE_FIFO_KHR;
}

VkExtent2D SwapChain::getSwapChainExtent() {
	const auto extent = m_swapChainSupportDetails.capabilities.currentExtent;
	m_size = { extent.width, extent.height };
	return extent;
}

uint32_t SwapChain::getSwapChainImageCount() const {
	uint32_t imageCount = m_swapChainSupportDetails.capabilities.minImageCount + 1;
	if (m_swapChainSupportDetails.capabilities.maxImageCount > 0 && imageCount > m_swapChainSupportDetails.capabilities.
		maxImageCount) {
		imageCount = m_swapChainSupportDetails.capabilities.maxImageCount;
		}
	return imageCount;
}

void SwapChain::createSwapChainImages() {
	uint32_t imageCount = 0;
	// images
	vkGetSwapchainImagesKHR(m_pLogicalDevice->GetHandle(), m_pHandle, &imageCount, nullptr);
	std::vector<VkImage> images(imageCount);
	vkGetSwapchainImagesKHR(m_pLogicalDevice->GetHandle(), m_pHandle, &imageCount, images.data());

	m_vecSwapChainImages.reserve(imageCount);
	for (auto image : images) {
		ImageCreateInfo info{
			.format = m_pSwapChainSurfaceFormat.format,
			.aspect = VK_IMAGE_ASPECT_COLOR_BIT,
		};
		m_vecSwapChainImages.emplace_back(new Image2D(image, info));
	    Graphics::Get()->GetCmdManager()->WithSingleCmdBuffer([&](CommandBuffer* pCmd) {
	        pCmd->TransitionImageLayout(m_vecSwapChainImages.back().get(), VK_IMAGE_LAYOUT_PRESENT_SRC_KHR);
	    });
	}
}

USING_ENGINE_NAMESPACE_END
