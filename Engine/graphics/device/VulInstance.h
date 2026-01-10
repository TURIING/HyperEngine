/********************************************************************************
* @author: TURIING
* @email: turiing@163.com
* @date: 2025/2/19 22:30
* @version: 1.0
* @description: 
********************************************************************************/

#ifndef VULINSTANCE_H
#define VULINSTANCE_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN

class VulInstance;

struct VulInstanceCreateInfo {
    bool enableValidationLayers = false;
    std::vector<const char *> instanceExtensions;
    std::vector<const char *> layers;
};

class VulInstanceBuilder {
public:
    VulInstanceBuilder& SetEnableValidationLayer(bool on);
    VulInstanceBuilder& AddExtensions(const char *ext);
    VulInstanceBuilder& AddLayer(const char* layer);
	[[nodiscard]] VulInstance* Build() const;

private:
    VulInstanceCreateInfo m_createInfo {};
};

class VulInstance final : public VulObject<VkInstance>{
public:
    explicit VulInstance(const VulInstanceCreateInfo &createInfo);
    ~VulInstance() override;
    NODISCARD bool IsEnableValidationLayers() const { return m_enableValidationLayers; }
    NODISCARD static VulInstanceBuilder Builder();

private:
    void createInstance(const VulInstanceCreateInfo &createInfo);
    NODISCARD static bool checkValidationLayerSupport(const std::vector<const char *> &layers) ;
    void setupDebugMessenger();
    static VkResult createDebugUtilsMessengerExt(VkInstance, const VkDebugUtilsMessengerCreateInfoEXT *, const VkAllocationCallbacks *, VkDebugUtilsMessengerEXT *);
    static void destroyDebugUtilsMessengerExt(VkInstance, VkDebugUtilsMessengerEXT, const VkAllocationCallbacks *);

private:
    VkDebugUtilsMessengerEXT m_pDebugMessenger = nullptr;
    bool m_enableValidationLayers = false;
};

USING_ENGINE_NAMESPACE_END
#endif //VULINSTANCE_H
