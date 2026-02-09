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

class Instance;

struct InstanceCreateInfo {
    bool enableValidationLayers = false;
    std::vector<const char *> instanceExtensions;
    std::vector<const char *> layers;
};

class InstanceBuilder {
public:
    InstanceBuilder& SetEnableValidationLayer(bool on);
    InstanceBuilder& AddExtensions(const char *ext);
    InstanceBuilder& AddLayer(const char* layer);
    NODISCARD Share<Instance> Build() const;

private:
    InstanceCreateInfo m_createInfo {};
};

class Instance final : public VulObject<VkInstance>{
public:
    explicit Instance(const InstanceCreateInfo &createInfo);
    ~Instance() override;
    NODISCARD bool IsEnableValidationLayers() const { return m_enableValidationLayers; }
    NODISCARD static InstanceBuilder Builder();

private:
    void createInstance(const InstanceCreateInfo &createInfo);
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
