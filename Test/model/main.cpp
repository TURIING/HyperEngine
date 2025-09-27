//
// Created by turiing on 2025/9/24.
//

#include "engine/Engine.h"
#include "MainApp.h"

int main() {
    auto engine = HyperEngine::Engine::Instance();
    engine->SetApp(std::make_unique<MainApp>());
    engine->Init();
    engine->Run();
}