//
// Created by turiing on 2026/2/4.
//

#ifndef HYPERENGINE_PRESENTMANAGER_H
#define HYPERENGINE_PRESENTMANAGER_H
#include "../../common/Common.h"

USING_ENGINE_NAMESPACE_BEGIN
class SwapChain;
class Semaphore;
class Surface;
class Image2D;
class Queue;

class PresentManager {
public:
    PresentManager();
    void Screen();
    NODISCARD Share<SwapChain> GetSwapChain(std::string_view name) { return m_mapSwapChains[name]; }
    NODISCARD const Share<Image2D>& GetCurrentImage(std::string_view name, u32 imageIndex);
    NODISCARD std::vector<Semaphore*> AcquireNextImage();
    NODISCARD std::vector<Semaphore*> GetPresentWaitSemaphores();
    NODISCARD u32 GetImageIndex() const { return m_imageIndex; }

private:
    NODISCARD Semaphore* acquireNextImage(std::string_view name, uint32_t &imageIndex);

private:
    std::unordered_map<std::string_view, Share<Surface>> m_mapSurfaces;
    std::unordered_map<std::string_view, Share<SwapChain>> m_mapSwapChains;
    int32_t m_frameIndex = -1;
    uint32_t m_imageIndex = 0;
    std::unordered_map<std::string_view, std::vector<Unique<Semaphore>>> m_mapImageAcquiredSemaphores;
    std::unordered_map<std::string_view, std::vector<Unique<Semaphore>>> m_mapPresentWaitSemaphores;
    Unique<Queue> m_pPresentQueue;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_PRESENTMANAGER_H