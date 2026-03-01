//
// Created by turiing on 2026/1/14.
//

#ifndef HYPERENGINE_GRAPHICSPIPELINE_H
#define HYPERENGINE_GRAPHICSPIPELINE_H

#include "../../common/Common.h"
#include "Pipeline.h"

USING_ENGINE_NAMESPACE_BEGIN

struct RenderEnvInfo: EnvInfo {
    DepthStencilInfo depthStencilInfo;
    RasterizationInfo rasterInfo{};
    MultiSampleInfo multiSampleInfo{};
    std::optional<std::vector<Shader::Define>> defines = std::nullopt;
    Pipeline::Stage stage;
};

class GraphicsPipeline: public Pipeline {
public:
    explicit GraphicsPipeline(const RenderEnvInfo& renderEnvInfo);
    ~GraphicsPipeline() override;

private:
    Stage m_stage;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_GRAPHICSPIPELINE_H