//
// Created by turiing on 2025/9/24.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../common/Common.h"
#include "../engine/Module.hpp"

USING_ENGINE_NAMESPACE_BEGIN
class VulInstance;
class VulPhysicalDevice;
class VulLogicDevice;

class ENGINE_EXPORT Graphics: public Module::Registrar<Graphics> {
    inline static const bool Registered = Register(Stage::Render);
public:
    Graphics();
    ~Graphics() override;

    void Update() override;
private:
    void createPipelineCache();

private:
    VulInstance* m_pInstance = nullptr;
    VulPhysicalDevice* m_pPhysicalDevice = nullptr;
    VulLogicDevice* m_pLogicDevice = nullptr;
    VkPipelineCache m_pPipelineCache = VK_NULL_HANDLE;
};

USING_ENGINE_NAMESPACE_END

#endif //GRAPHICS_H
