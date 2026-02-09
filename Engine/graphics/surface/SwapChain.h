//
// Created by turiing on 2026/1/15.
//

#ifndef HYPERENGINE_SWAPCHAIN_H
#define HYPERENGINE_SWAPCHAIN_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN
class Surface;
class PhysicalDevice;
class LogicDevice;
class Image2D;
class Semaphore;

class SwapChain : public VulObject<VkSwapchainKHR> {
    struct SwapChainSupportDetails {
        VkSurfaceCapabilitiesKHR		capabilities{};
        std::vector<VkSurfaceFormatKHR> formats;
        std::vector<VkPresentModeKHR>	presentModes;
    };
public:
    SwapChain(const Share<Surface> &pSurface, const Share<SwapChain> &pOldSwapChain = nullptr);
    ~SwapChain() override;
    NODISCARD Size2DUI GetSize() const { return m_size; }
    NODISCARD uint32_t GetImageCount() const { return m_vecSwapChainImages.size(); }
    NODISCARD VkResult AcquireNextImage(Semaphore* pSemaphore, uint32_t &imageIndex) const;
    NODISCARD const Share<Image2D>& GetImage(uint32_t imageIndex) { return m_vecSwapChainImages[imageIndex]; }

private:
    void acquireSwapChainSupportDetails();
    void chooseSwapSurfaceFormat();
    NODISCARD VkPresentModeKHR chooseSwapPresentMode() const;
    NODISCARD VkExtent2D getSwapChainExtent();
    NODISCARD uint32_t getSwapChainImageCount() const;
    void createSwapChainImages();

private:
    Share<PhysicalDevice> m_pPhysicalDevice = nullptr;
    Share<Surface> m_pSurface = nullptr;
    Share<LogicDevice> m_pLogicalDevice = nullptr;

    Size2DUI m_size;

    VkSurfaceTransformFlagsKHR m_preTransform;
    VkCompositeAlphaFlagBitsKHR m_compositeAlpha;
    std::vector<Share<Image2D>> m_vecSwapChainImages;

    SwapChainSupportDetails	m_swapChainSupportDetails;
    VkSurfaceFormatKHR m_pSwapChainSurfaceFormat{};
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_SWAPCHAIN_H