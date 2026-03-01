//
// Created by turiing on 2026/1/30.
//

#ifndef HYPERENGINE_MAINRENDERER_H
#define HYPERENGINE_MAINRENDERER_H
#include <graphics/pipeline/Renderer.h>
using namespace HyperEngine;

class MainRenderer : public Renderer {
public:
    MainRenderer();

    void Start() override;

    void Update() override;
};


#endif //HYPERENGINE_MAINRENDERER_H