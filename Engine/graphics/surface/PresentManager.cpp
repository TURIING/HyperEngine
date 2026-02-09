//
// Created by turiing on 2026/2/4.
//

#include "PresentManager.h"
#include "../Graphics.h"
#include "Surface.h"
#include "SwapChain.h"
#include "../../device/Windows.h"
#include "../../device/Window.h"
#include "../sync/Semaphore.h"
#include "../device/Queue.h"
#include "../device/PhysicalDevice.h"

USING_ENGINE_NAMESPACE_BEGIN
PresentManager::PresentManager() {
    Windows::Get()->OnAddWindow().connect(this, [this](const Share<Window> &pWindow) {
        auto name = pWindow->GetName();
        m_mapSurfaces[name] = std::make_shared<Surface>(pWindow);
        m_mapSwapChains[name] = std::make_shared<SwapChain>(m_mapSurfaces[name]);
        for (auto i = 0; i < m_mapSwapChains[name]->GetImageCount(); i++) {
            m_mapImageAcquiredSemaphores[name].emplace_back(std::make_unique<Semaphore>());
            m_mapPresentWaitSemaphores[name].emplace_back(std::make_unique<Semaphore>());
        }
    });
    m_pPresentQueue = std::make_unique<Queue>(Graphics::Get()->GetPhysicalDevice()->GetQueueFamily(QueueType::Present));
    LOG_DEBUG("PresentManager created");
}

void PresentManager::Screen() {
    std::vector<SwapChain*> swapChains;
    for (const auto &[name, swapChain]: m_mapSwapChains) {
        swapChains.push_back(swapChain.get());
    }

    auto semaphores = GetPresentWaitSemaphores();

    std::vector<u32> imageIndices;
    for (auto i = 0; i < swapChains.size(); i++) {
        imageIndices.push_back(m_imageIndex);
    }

    QueuePresentInfo presentInfo;
    presentInfo.pSwapChain = swapChains.data();
    presentInfo.swapChainCount = swapChains.size();
    presentInfo.imageIndex = imageIndices.data();
    presentInfo.pWaitSemaphores = semaphores.data();
    presentInfo.waitSemaphoreCount = semaphores.size();
    m_pPresentQueue->Present(presentInfo);
}

const Share<Image2D>& PresentManager::GetCurrentImage(std::string_view name, u32 imageIndex) {
    return m_mapSwapChains[name]->GetImage(imageIndex);
}

std::vector<Semaphore *> PresentManager::AcquireNextImage() {
    std::vector<Semaphore *> result;
    for (auto &[name, pSwapChain]: m_mapSwapChains) {
        result.push_back(acquireNextImage(name, m_imageIndex));
    }
    return result;
}

std::vector<Semaphore *> PresentManager::GetPresentWaitSemaphores() {
    std::vector<Semaphore *> result;
    for (const auto &[name, vecSemaphore]: m_mapPresentWaitSemaphores) {
        result.push_back(vecSemaphore[m_imageIndex].get());
    }
    return result;
}

Semaphore* PresentManager::acquireNextImage(std::string_view name, uint32_t &imageIndex) {
    auto &pSwapChain = m_mapSwapChains[name];
    m_frameIndex = (m_frameIndex + 1) % pSwapChain->GetImageCount();
    const auto &pSemaphore = m_mapImageAcquiredSemaphores[name][m_frameIndex];
    auto result = pSwapChain->AcquireNextImage(pSemaphore.get(), imageIndex);
    if (result == VK_ERROR_OUT_OF_DATE_KHR) {
        TODO
    }
    else if (result != VK_SUCCESS && result != VK_SUBOPTIMAL_KHR) {
        LOG_CRITICAL("Failed to acquire swap chain image!");
    }
    return pSemaphore.get();
}

USING_ENGINE_NAMESPACE_END
