//
// Created by turiing on 2025/9/24.
//

#include "Graphics.h"

USING_ENGINE_NAMESPACE_BEGIN

Graphics::Graphics() {
    m_pGpuFactory = new HyperGpu::GpuFactory(HyperGpu::GpuFactory::VULKAN);

    HyperGpu::QueueInfo queueInfos[] = {
        { HyperGpu::QueueType::Graphics, 1.0 },
        { HyperGpu::QueueType::Present, 0.8 }
    };
    const HyperGpu::DeviceCreateInfo deviceInfo{
        .pQueueInfo = queueInfos,
        .queueInfoCount = std::size(queueInfos)
    };
    m_pGpuDevice = m_pGpuFactory->CreateDevice(deviceInfo);

    LOG_INFO("Graphics Module Created.");
}

Graphics::~Graphics() {
    m_pGpuFactory->DestroyDevice(m_pGpuDevice);
    m_pGpuFactory->SubRef();
}

void Graphics::Update() {
}

USING_ENGINE_NAMESPACE_END
