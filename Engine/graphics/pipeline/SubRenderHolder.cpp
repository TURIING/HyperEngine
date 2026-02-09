//
// Created by turiing on 2026/1/17.
//

#include "SubRenderHolder.h"

USING_ENGINE_NAMESPACE_BEGIN
void SubRenderHolder::Clear() {
    m_mapStages.clear();
}

void SubRenderHolder::removeSubRenderStage(const TypeId &id) {
    for (auto it = m_mapStages.begin(); it != m_mapStages.end();) {
        if (it->second == id) {
            it = m_mapStages.erase(it);
        } else {
            ++it;
        }
    }
}

void SubRenderHolder::RenderStage(const Pipeline::Stage &stage, CommandBuffer *pCommandBuffer) {
    for (const auto &[stageIndex, typeId] : m_mapStages) {
        if (stageIndex.first != stage) {
            continue;
        }

        if (auto &subRender = m_mapSubRenders[typeId]) {
            subRender->Render(pCommandBuffer);
        }
    }
}

USING_ENGINE_NAMESPACE_END
