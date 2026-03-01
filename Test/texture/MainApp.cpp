//
// Created by turiing on 2025/9/24.
//

#include "MainApp.h"
#include <device/Windows.h>
#include "MainRenderer.h"
#include <graphics/Graphics.h>

using namespace HyperEngine;

MainApp::MainApp(): App("Triangle") {
}

MainApp::~MainApp() {
}

void MainApp::Init() {
    Windows::Get()->AddWindow("MainWindow");
    Graphics::Get()->SetRenderer(std::make_unique<MainRenderer>());
}

void MainApp::Update() {
}
