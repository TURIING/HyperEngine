//
// Created by turiing on 2025/9/24.
//

#ifndef GRAPHICS_H
#define GRAPHICS_H

#include "../common/Common.h"
#include "../engine/Module.hpp"
#include "../device/Windows.h"

USING_ENGINE_NAMESPACE_BEGIN
class Instance;
class PhysicalDevice;
class LogicDevice;
class CommandPool;
class CmdManager;
class DescriptorPool;
class ResourceCache;
class Renderer;
class RenderStage;
class PresentManager;
class CommandBuffer;
class Queue;
class Fence;

class ENGINE_EXPORT Graphics: public Module::Registrar<Graphics> {
    inline static const bool Registered = Register(Stage::Render, Requires<Windows>());
public:
    Graphics();
    ~Graphics() override;
    void Update() override;
    void Init() override;
    void SetRenderer(Unique<Renderer> &&renderer);
    NODISCARD const Share<Instance>& GetInstance() const { return m_pInstance; }
    NODISCARD const Share<LogicDevice>& GetLogicDevice() const { return m_pLogicDevice; }
    NODISCARD const Share<PhysicalDevice>& GetPhysicalDevice() const { return m_pPhysicalDevice; }
    NODISCARD VkPipelineCache GetPipelineCache() const { return m_pPipelineCache; }
    NODISCARD const Share<CmdManager> &GetCmdManager() const { return m_pCmdManager; }
    NODISCARD const Share<ResourceCache> &GetResourceCache() const { return m_pResourceCache; }
    NODISCARD RenderStage* GetRenderStage(u32 stageIndex) const;
    NODISCARD const Share<PresentManager> &GetPresentManager() const { return m_pPresentManager; }
    NODISCARD DescriptorPool* GetDescriptorPool() const { return m_pDescriptorPool.get(); }

private:
    void createPipelineCache();
    void startRenderPass(RenderStage* pRenderStage, CommandBuffer* pCmd) const;

private:
    Share<Instance> m_pInstance = nullptr;
    Share<PhysicalDevice> m_pPhysicalDevice = nullptr;
    Share<LogicDevice> m_pLogicDevice = nullptr;
    VkPipelineCache m_pPipelineCache = VK_NULL_HANDLE;
    Share<CmdManager> m_pCmdManager;
    Unique<DescriptorPool> m_pDescriptorPool;
    Share<ResourceCache> m_pResourceCache;
    Unique<Renderer> m_pMainRenderer;
    Share<PresentManager> m_pPresentManager;
    std::vector<Unique<CommandBuffer>> m_vecCommandBuffers;
    std::vector<Unique<Fence>> m_vecRenderFences;
    Unique<Queue> m_pSubmitQueue;
};

USING_ENGINE_NAMESPACE_END

#endif //GRAPHICS_H
