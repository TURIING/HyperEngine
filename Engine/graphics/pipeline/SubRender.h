//
// Created by turiing on 2026/1/16.
//

#ifndef HYPERENGINE_SUBRENDER_H
#define HYPERENGINE_SUBRENDER_H
#include "../../common/Common.h"
#include "Pipeline.h"
#include "../command/CommandBuffer.h"

USING_ENGINE_NAMESPACE_BEGIN

class SubRender: NoCopyable {
public:
    explicit SubRender(const Pipeline::Stage &stage): m_stage(stage){}
    ~SubRender() override = default;
    virtual void Render(CommandBuffer *pCmd) = 0;
    NODISCARD const Pipeline::Stage &GetStage() const { return m_stage; }
    NODISCARD bool IsEnabled() const { return m_enabled; }
    void SetEnabled(bool enable) { this->m_enabled = enable; }

private:
    bool m_enabled = false;
    Pipeline::Stage m_stage;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_SUBRENDER_H