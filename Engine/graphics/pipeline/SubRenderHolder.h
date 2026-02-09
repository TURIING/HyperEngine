//
// Created by turiing on 2026/1/17.
//

#ifndef HYPERENGINE_SUBRENDERHOLDER_H
#define HYPERENGINE_SUBRENDERHOLDER_H
#include "../../common/Common.h"
#include "SubRender.h"

USING_ENGINE_NAMESPACE_BEGIN

class SubRenderHolder : NoCopyable {
public:
    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T*, SubRender*>>>
    NODISCARD bool Has() const {
        const auto it = m_mapSubRenders.find(TypeInfo<SubRender>::GetTypeId<T>());
        return it != m_mapSubRenders.end() && it->second;
    }

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T*, SubRender*>>>
    NODISCARD T* Get() const {
        const auto typeId = TypeInfo<SubRender>::GetTypeId<T>();
        if (auto it = m_mapSubRenders.find(typeId); it != m_mapSubRenders.end() && it->second) {
            return static_cast<T*>(it->second.get());
        }
        return nullptr;
    }

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T*, SubRender*>>>
    NODISCARD T* Add(const Pipeline::Stage &stage, std::unique_ptr<T> &&subRender) {
        const auto typeId = TypeInfo<SubRender>::GetTypeId<T>();
        m_mapStages.insert({ StageIndex(stage, m_mapSubRenders.size()), typeId});
        m_mapSubRenders[typeId] = std::move(subRender);
        return static_cast<T*>(m_mapSubRenders[typeId].get());
    }

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T*, SubRender*>>>
    void Remove() {
        const auto typeId = TypeInfo<SubRender>::GetTypeId<T>();
        removeSubRenderStage(typeId);
        m_mapSubRenders.erase(typeId);
    }

    void Clear();

    void RenderStage(const Pipeline::Stage &stage, CommandBuffer *pCommandBuffer);
private:
    void removeSubRenderStage(const TypeId &id);

private:
	using StageIndex = std::pair<Pipeline::Stage, std::size_t>;
    std::unordered_map<TypeId, std::unique_ptr<SubRender>> m_mapSubRenders;
    std::multimap<StageIndex, TypeId> m_mapStages;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_SUBRENDERHOLDER_H