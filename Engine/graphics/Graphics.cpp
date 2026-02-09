//
// Created by turiing on 2025/9/24.
//

#include "Graphics.h"
#include <SPIRV/GlslangToSpv.h>
#include "device/Instance.h"
#include "device/PhysicalDevice.h"
#include "device/LogicDevice.h"
#include "glslang/glslang/Public/ShaderLang.h"
#include "command/CmdManager.h"
#include "pipeline/Renderer.h"
#include "pipeline/RenderPass.h"
#include "resource/descriptor/DescriptorPool.h"
#include "resource/ResourceCache.h"
#include "resource/FrameBuffer.h"
#include "surface/PresentManager.h"
#include "device/Queue.h"
#include "sync/Fence.h"
#include "sync/Semaphore.h"

USING_ENGINE_NAMESPACE_BEGIN
#ifdef NODEBUG
constexpr bool ENABLE_VALIDATION_LAYER = false;
#else
constexpr bool ENABLE_VALIDATION_LAYER = true;
#endif

Graphics::Graphics() {
    m_pInstance = Instance::Builder()
                    .SetEnableValidationLayer(ENABLE_VALIDATION_LAYER)
                    .AddExtensions("VK_KHR_surface")
#ifndef NODEBUG
                    .AddExtensions(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
#endif
#if PLATFORM_WINDOWS
                    .AddExtensions("VK_KHR_win32_surface")
#elif PLATFORM_MACOS
                    .AddExtensions("VK_EXT_metal_surface")
                    .AddExtensions(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)
                    .AddExtensions(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
#elif PLATFORM_IOS
                    .AddExtensions("VK_EXT_metal_surface")
                    .AddExtensions(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)
                    .AddExtensions(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
#endif
                    .Build();

    m_pPhysicalDevice = PhysicalDevice::Builder()
                            .SetInstance(m_pInstance)
                            .AddExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
                            .AddExtension("VK_KHR_portability_subset")
                            .Build();

    constexpr VkPhysicalDeviceFeatures deviceFeatures{
        .samplerAnisotropy = VK_TRUE,
    };
    QueueInfo queueInfos[] = {
        { QueueType::Graphics, 1.0 },
        { QueueType::Present, 0.8 }
    };
    m_pLogicDevice = LogicDevice::Builder()
                        .SetInstance(m_pInstance)
                        .SetPhysicalDevice(m_pPhysicalDevice)
                        .SetQueueInfos(queueInfos, std::size(queueInfos))
                        .AddExtension(VK_KHR_SWAPCHAIN_EXTENSION_NAME)
                        .AddExtension("VK_KHR_portability_subset")
                        .SetDeviceFeatures(deviceFeatures)
                        .Build();

    createPipelineCache();

    if (!glslang::InitializeProcess()) {
        LOG_CRITICAL("Failed to initialize glslang process.");
    }

    LOG_INFO("Graphics Module Created.");
}

Graphics::~Graphics() {
}


void Graphics::Init() {
    m_pCmdManager = std::make_shared<CmdManager>();
    m_pDescriptorPool = std::make_shared<DescriptorPool>();
    m_pResourceCache = std::make_shared<ResourceCache>();
    m_pPresentManager = std::make_shared<PresentManager>();
    m_pSubmitQueue = std::make_unique<Queue>(m_pPhysicalDevice->GetQueueFamily(QueueType::Graphics));
    for (auto i = 0; i < 3; i++) {
        m_vecCommandBuffers.emplace_back(std::unique_ptr<CommandBuffer>(m_pCmdManager->CreateCommandBuffer()));
        m_vecRenderFences.emplace_back(std::make_unique<Fence>());
        m_vecRenderFences.back()->Reset();
    }
}

RenderStage* Graphics::GetRenderStage(u32 stageIndex) const {
    return m_pMainRenderer->GetRenderStage(stageIndex);
}

void Graphics::createPipelineCache() {
    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    CALL_VK(vkCreatePipelineCache(m_pLogicDevice->GetHandle(), &pipelineCacheCreateInfo, nullptr, &m_pPipelineCache));
}

void Graphics::startRenderPass(RenderStage *pRenderStage, CommandBuffer* pCmd) const {
    const auto viewport = pRenderStage->GetViewport();

    BeginRenderPassInfo info;
    info.renderPass = pRenderStage->GetRenderPass()->GetHandle();
    info.framebuffer = pRenderStage->GetFrameBuffer()->GetHandle();
    info.renderArea = {{viewport.offset.x,viewport.offset.y}, { viewport.size.width, viewport.size.height}};
    info.clearValues = &pRenderStage->GetClearValues();
    pCmd->BeginRenderPass(info);
    pCmd->SetViewport(pRenderStage->GetViewport());
    pCmd->SetScissor(pRenderStage->GetScissor());
}

void Graphics::Update() {
    if (!m_pMainRenderer) return;
    if (!m_pMainRenderer->m_started) {
        m_pMainRenderer->Start();
        m_pMainRenderer->m_started = true;
    }

    auto pCmd = m_vecCommandBuffers.at(m_pPresentManager->GetImageIndex()).get();
    auto pRenderFence = m_vecRenderFences.at(m_pPresentManager->GetImageIndex()).get();

    pRenderFence->Reset();
    auto vecImageAvailableSemaphore = m_pPresentManager->AcquireNextImage();

    m_pMainRenderer->Update();
    pCmd->Reset();
    pCmd->Begin();

    Pipeline::Stage stage {};
    for (auto &renderStage: m_pMainRenderer->m_vecRenderStages) {
        startRenderPass(renderStage.get(), pCmd);

        for (const auto &subpass: renderStage->GetSubpasses()) {
            stage.subpassIndex = subpass.binding;
            m_pMainRenderer->m_subRenderHolder.RenderStage(stage, pCmd);
            if (subpass.binding != renderStage->GetSubpasses().back().binding) {
                pCmd->NextSubpass();
            }
        }

        pCmd->EndRenderPass();
        stage.renderStageIndex++;
    }

    pCmd->End();

    auto vecPresentWaitSemaphores = m_pPresentManager->GetPresentWaitSemaphores();
    QueueSubmitInfo submitInfo;
    submitInfo.waitSemaphoreCount = vecImageAvailableSemaphore.size();
    submitInfo.pWaitSemaphores = vecImageAvailableSemaphore.data();
    submitInfo.pCmd = (CommandBuffer*[])pCmd;
    submitInfo.cmdCount = 1;
    submitInfo.pSignalSemaphores = vecPresentWaitSemaphores.data();
    submitInfo.signalSemaphoreCount = vecPresentWaitSemaphores.size();
    submitInfo.pFence = pRenderFence;
    m_pSubmitQueue->Submit(submitInfo);
    pRenderFence->Wait();

    m_pPresentManager->Screen();
}

void Graphics::SetRenderer(Unique<Renderer> &&renderer) {
    m_pMainRenderer = std::move(renderer);
}

USING_ENGINE_NAMESPACE_END
