//
// Created by turiing on 2026/1/17.
//

#ifndef HYPERENGINE_RENDERER_H
#define HYPERENGINE_RENDERER_H
#include "../../common/Common.h"
#include "SubRenderHolder.h"
#include "RenderStage.h"

USING_ENGINE_NAMESPACE_BEGIN

class Renderer {
    friend class Graphics;
public:
    Renderer() = default;
    virtual ~Renderer() = default;

    virtual void Start() = 0;
    virtual void Update() = 0;

    template<typename T>
    NODISCARD bool HasSubRender() const {
        return m_subRenderHolder.Has<T>();
    }

    template<typename T>
    NODISCARD T* GetSubRender() const {
        return m_subRenderHolder.Get<T>();
    }

    template<typename T, typename... Args>
    T* AddSubRender(const Pipeline::Stage &stage, Args &&...args) {
        return m_subRenderHolder.Add<T>(stage, std::make_unique<T>(stage, std::forward<Args>(args)...));
    }

    template<typename T>
    void RemoveSubRender() {
        m_subRenderHolder.Remove<T>();
    }

    void ClearSubRenders() {
        m_subRenderHolder.Clear();
    }

    NODISCARD RenderStage *GetRenderStage(uint32_t index) const {
        if (m_vecRenderStages.empty() || m_vecRenderStages.size() < index)
            return nullptr;

        return m_vecRenderStages.at(index).get();
    }

    void AddRenderStage(std::unique_ptr<RenderStage> &&renderStage) {
        m_vecRenderStages.emplace_back(std::move(renderStage));
    }

private:
    bool m_started = false;
    std::vector<Unique<RenderStage>> m_vecRenderStages;
    SubRenderHolder m_subRenderHolder;
};

USING_ENGINE_NAMESPACE_END

#endif //HYPERENGINE_RENDERER_H