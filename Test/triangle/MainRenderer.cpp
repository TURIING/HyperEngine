//
// Created by turiing on 2026/1/30.
//

#include "MainRenderer.h"
#include <graphics/pipeline/SubRender.h>
#include <graphics/pipeline/GraphicsPipeline.h>
#include <model/Model.hpp>

class TriangleSubRender: public SubRender {
public:
    explicit TriangleSubRender(const Pipeline::Stage &stage, Unique<Model> &model): SubRender(stage), m_model(std::move(model)) {
        RenderEnvInfo info {};
        info.shaders = { "shaders/triangle/tri1.vert", "shaders/triangle/tri1.frag" };
        m_pipeline = std::make_unique<GraphicsPipeline>(info);
    }

    void Render(CommandBuffer *pCmd) override {
        pCmd->BindPipeline(m_pipeline.get());
        m_model->Render(pCmd);
    }

private:
    Unique<GraphicsPipeline> m_pipeline;
    Unique<Model> m_model;
};

MainRenderer::MainRenderer() {
    std::vector<Attachment> attachments {
        {0, "MainWindow", AttachmentType::SwapChain, PixelFormat::B8G8R8A8_SRGB },
    };

    std::vector<SubpassInfo> subpasses {
        {0, { 0 }},
    };

    AddRenderStage(std::make_unique<RenderStage>(attachments, subpasses));
}

void MainRenderer::Start() {
    struct Vertex {
        glm::vec2 position;
        glm::vec3 color;
    };
    std::vector<Vertex> vertices {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };
    auto model = std::make_unique<Model>(vertices);
    AddSubRender<TriangleSubRender>( {0, 0}, model);
}

void MainRenderer::Update() {
}
