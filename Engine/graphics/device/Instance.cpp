/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/19 22:32
* @version: 1.0
* @description: 
********************************************************************************/

#include "Instance.h"

USING_ENGINE_NAMESPACE_BEGIN

VkBool32 VKAPI_CALL debugCallBack(VkDebugUtilsMessageSeverityFlagBitsEXT messageServerity,
                                  VkDebugUtilsMessageTypeFlagsEXT messageType,
                                  const VkDebugUtilsMessengerCallbackDataEXT *pCallBackData,
                                  void *pUserData) {
    if (messageServerity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
        LOG_WARNING("Vulkan validation layer: {}", pCallBackData->pMessage);
    }
    else if (messageServerity == VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT)
    {
        LOG_CRITICAL("Vulkan validation layer: {}", pCallBackData->pMessage);
    }
    return VK_FALSE;
}

Instance::Instance(const InstanceCreateInfo &createInfo): m_enableValidationLayers(createInfo.enableValidationLayers) {
    this->createInstance(createInfo);
    this->setupDebugMessenger();
}

Instance::~Instance() {
    LOG_ASSERT(m_pHandle != nullptr);

    if (m_enableValidationLayers) {
        destroyDebugUtilsMessengerExt(m_pHandle, m_pDebugMessenger, nullptr);
    }

    vkDestroyInstance(m_pHandle, nullptr);
}

InstanceBuilder Instance::Builder() {
    return InstanceBuilder {};
}

void Instance::createInstance(const InstanceCreateInfo &info) {
    CALL_VK(volkInitialize());

    if(info.enableValidationLayers && !checkValidationLayerSupport(info.layers)) {
        LOG_CRITICAL("validation layers requested, but not available!");
    }

    // appInfo
    VkApplicationInfo appInfo = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "HyperEngine",
        .applicationVersion = VK_MAKE_VERSION(1, 0, 0),
        .pEngineName = "HyperEngine",
        .engineVersion = VK_MAKE_VERSION(1, 0, 0),
        .apiVersion = VK_API_VERSION_1_4,
    };


    // createInfo
    VkInstanceCreateInfo createInfo = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
#if PLATFORM_MACOS || PLATFORM_IOS
        .flags = VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR,
#endif
        .pApplicationInfo = &appInfo,
        .enabledExtensionCount = static_cast<uint32_t>(info.instanceExtensions.size()),
        .ppEnabledExtensionNames = info.instanceExtensions.data()
    };
    if(info.enableValidationLayers) {
        VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{
            .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
            .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
            .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
            .pfnUserCallback = debugCallBack,
            .pUserData = nullptr,
        };

        createInfo.enabledLayerCount = static_cast<uint32_t>(info.layers.size());
        createInfo.ppEnabledLayerNames = info.layers.data();
        createInfo.pNext = (VkDebugUtilsMessengerCreateInfoEXT *)&debugCreateInfo;
    }
    else {
        createInfo.enabledLayerCount = 0;
        createInfo.pNext = nullptr;
    }

    CALL_VK(vkCreateInstance(&createInfo, nullptr, &m_pHandle));
    LOG_DEBUG("Vulkan Instance Created");

    volkLoadInstance(m_pHandle);
}

bool Instance::checkValidationLayerSupport(const std::vector<const char *> &layers) {
    uint32_t layerCount;
    vkEnumerateInstanceLayerProperties(&layerCount, nullptr);

    std::vector<VkLayerProperties> availableLayers(layerCount);
    vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

    for (const char* layerName : layers) {
        bool layerFound = false;
        for (const auto& layerProperties : availableLayers) {
            if (strcmp(layerName, layerProperties.layerName) == 0) {
                layerFound = true;
                break;
            }
        }

        if(!layerFound) {
            return false;
        }
    }

    return true;
}

void Instance::setupDebugMessenger() {
    if (!m_enableValidationLayers) return;

    VkDebugUtilsMessengerCreateInfoEXT debugCreateInfo{
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .pNext = nullptr,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = debugCallBack,
        .pUserData = nullptr,
    };

    CALL_VK(createDebugUtilsMessengerExt(m_pHandle, &debugCreateInfo, nullptr, &m_pDebugMessenger));
}

/**
 * 载入vkCreateDebugUtilsMessengerEXT函数
 * @param instance
 * @param pCreateInfo
 * @param pAllocator
 * @param pDebugMessenger
 * @return
 */
VkResult Instance::createDebugUtilsMessengerExt(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
                                        const VkAllocationCallbacks *pAllocator,
                                        VkDebugUtilsMessengerEXT *pDebugMessenger) {
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if (func != nullptr) {
        return func(instance, pCreateInfo, pAllocator, pDebugMessenger);
    } else {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void Instance::destroyDebugUtilsMessengerExt(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks *pAllocator) {
    auto func = (PFN_vkDestroyDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if (func != nullptr) {
        func(instance, debugMessenger, pAllocator);
    }
}

InstanceBuilder& InstanceBuilder::SetEnableValidationLayer(bool on) {
    m_createInfo.enableValidationLayers = on;
    m_createInfo.layers.push_back("VK_LAYER_KHRONOS_validation");
    m_createInfo.instanceExtensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    return *this;
}

InstanceBuilder& InstanceBuilder::AddExtensions(const char *ext) {
    m_createInfo.instanceExtensions.push_back(ext);
    return *this;
}

InstanceBuilder& InstanceBuilder::AddLayer(const char* layer) {
    m_createInfo.layers.push_back(layer);
    return *this;
}

Share<Instance> InstanceBuilder::Build() const {
    return std::make_shared<Instance>(m_createInfo);
}

USING_ENGINE_NAMESPACE_END