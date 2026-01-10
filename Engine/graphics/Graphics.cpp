//
// Created by turiing on 2025/9/24.
//

#include "Graphics.h"
#include <SPIRV/GlslangToSpv.h>
#include "device/VulInstance.h"
#include "device/VulPhysicalDevice.h"
#include "device/VulLogicDevice.h"
#include "glslang/glslang/Public/ShaderLang.h"

USING_ENGINE_NAMESPACE_BEGIN
#ifdef NODEBUG
constexpr bool ENABLE_VALIDATION_LAYER = false;
#else
constexpr bool ENABLE_VALIDATION_LAYER = true;
#endif

Graphics::Graphics() {
    m_pInstance = VulInstance::Builder()
                    .SetEnableValidationLayer(ENABLE_VALIDATION_LAYER)
                    .AddExtensions("VK_KHR_surface")
#ifndef NODEBUG
                    .AddExtensions(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)
#endif
#if PLATFORM_WINDOWS
                    .AddExtensions("VK_KHR_win32_surface")
#elif PLATFORM_MACOS
                    .AddExtensions("VK_MVK_macos_surface")
                    .AddExtensions(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)
                    .AddExtensions(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
#elif PLATFORM_IOS
                    .AddExtensions("VK_EXT_metal_surface")
                    .AddExtensions(VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME)
                    .AddExtensions(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME)
#endif
                    .Build();

    m_pPhysicalDevice = VulPhysicalDevice::Builder()
                            .SetVulInstance(m_pInstance)
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
    m_pLogicDevice = VulLogicDevice::Builder()
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
    m_pPhysicalDevice->SubRef();
    m_pLogicDevice->SubRef();
    m_pInstance->SubRef();
}

void Graphics::createPipelineCache() {
    VkPipelineCacheCreateInfo pipelineCacheCreateInfo = {};
    pipelineCacheCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_CACHE_CREATE_INFO;
    CALL_VK(vkCreatePipelineCache(m_pLogicDevice->GetHandle(), &pipelineCacheCreateInfo, nullptr, &m_pPipelineCache));
}

void Graphics::Update() {
}


USING_ENGINE_NAMESPACE_END
