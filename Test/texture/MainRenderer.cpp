//
// Created by turiing on 2026/1/30.
//

#include "MainRenderer.h"
#include <graphics/pipeline/SubRender.h>
#include <graphics/pipeline/GraphicsPipeline.h>
#include <model/Model.hpp>
#include <graphics/resource/descriptor/DescriptorHandler.h>
#include <graphics/resource/image/Image2D.h>

class TriangleSubRender: public SubRender {
public:
    explicit TriangleSubRender(const Pipeline::Stage &stage, Unique<Model> &model): SubRender(stage), m_model(std::move(model)) {
        RenderEnvInfo info {};
        info.shaders = { "shaders/texture/tex.vert", "shaders/texture/tex.frag" };
        info.rasterInfo.primitiveType = PrimitiveType::TRIANGLE_STRIP;
        m_pipeline = std::make_unique<GraphicsPipeline>(info);

        m_image = std::unique_ptr<Image2D>(Image2D::Create("textures/texture1.png"));

        m_descriptorHandler = std::make_unique<DescriptorHandler>();
        m_descriptorHandler->BindPipeline(m_pipeline.get());
        m_descriptorHandler->Push("texture0", m_image.get());
        m_descriptorHandler->Update();
    }

    void Render(CommandBuffer *pCmd) override {
        pCmd->BindPipeline(m_pipeline.get());
        m_descriptorHandler->BindDescriptor(pCmd);
        m_model->Render(pCmd);
    }

private:
    Unique<GraphicsPipeline> m_pipeline;
    Unique<Model> m_model;
    Unique<DescriptorHandler> m_descriptorHandler;
    Unique<Image2D> m_image;
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
        glm::vec2 texCoord;
    };
    std::vector<Vertex> vertices {
        { { -0.5, -0.5 }, { 0.0, 0.0} },
        { {0.5, -0.5 }, { 1.0, 0.0 } },
        { { -0.5, 0.5 }, {0.0, 1.0 } },
        { { 0.5, 0.5 }, { 1.0, 1.0 } }
    };
    auto model = std::make_unique<Model>(vertices);
    AddSubRender<TriangleSubRender>( {0, 0}, model);
}

void MainRenderer::Update() {
}
