//
// Created by turiing on 2026/1/14.
//

#ifndef HYPERENGINE_GRAPHICSPIPELINE_H
#define HYPERENGINE_GRAPHICSPIPELINE_H

#include "../../common/Common.h"
#include "Pipeline.h"

USING_ENGINE_NAMESPACE_BEGIN
class Shader;
class DescriptorSetLayout;
class PipelineLayout;

struct RenderEnvInfo: EnvInfo {
    DepthStencilInfo depthStencilInfo;
    RasterizationInfo rasterInfo{};
    MultiSampleInfo multiSampleInfo{};
    std::vector<Shader::Define> defines = {};
    Pipeline::Stage stage;
};

class GraphicsPipeline: public Pipeline {
public:
    explicit GraphicsPipeline(const RenderEnvInfo& renderEnvInfo);
    ~GraphicsPipeline() override;

private:
    Stage m_stage;
    Share<Shader> m_pShader;
    Share<DescriptorSetLayout> m_pDescriptorSetLayout;
    Unique<PipelineLayout> m_pPipelineLayout;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_GRAPHICSPIPELINE_H