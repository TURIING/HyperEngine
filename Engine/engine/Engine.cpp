//
// Created by turiing on 2025/8/7.
//

#include "Engine.h"
#include "Config.hpp"

USING_ENGINE_NAMESPACE_BEGIN

Engine* Engine::Instance() {
    static Engine engine;
    return &engine;
}

void Engine::Init(const std::string &argv0, const ModuleFilter& filter) {
    Singleton<LogManager>::GetInstance()->Init();
    m_argv0 = argv0;

    LOG_INFO("Version: {}", HYPER_VERSION);
    LOG_INFO("Git Commit: {}, on {} branch", HYPER_COMPILED_COMMIT_HASH, HYPER_COMPILED_BRANCH);
    LOG_INFO("Build on {}, in {} Mode", HYPER_COMPILED_SYSTEM, HYPER_BUILD_TYPE);

    LOG_INFO("Begin create modules.");
    for (auto it = Module::Registry().begin(); it != Module::Registry().end(); ++it) {
        createModule(it, filter);
    }
    LOG_INFO("End create modules.");
}

void Engine::Run() {
    LOG_ASSERT(m_pApp);
    m_pApp->Init();

    m_elapsedRender.SetInterval(Time::Seconds(1.0f / 60.0f));
    m_elapsedUpdate.SetInterval(Time::Seconds(1.0f / 60.0f));

    LOG_INFO("Engine running.");

    while (m_running) {
        m_pApp->Update();

        updateStage(Module::Stage::Always);

        if (m_elapsedUpdate.GetElapsed() != 0) {
            updateStage(Module::Stage::Pre);
            updateStage(Module::Stage::Normal);
            updateStage(Module::Stage::Post);
        }

        if (m_elapsedRender.GetElapsed() != 0) {
            updateStage(Module::Stage::Render);
        }
    }

    LOG_INFO("Engine stopped.");
}

Engine::~Engine() {
    for (auto it = m_modules.begin(); it != m_modules.end(); ++it) {
        destroyModule(it->first);
    }
}

void Engine::createModule(const Module::RegistryMap::const_iterator it, const ModuleFilter &filter) {
    if (m_modules.find(it->first) != m_modules.end()) {
        return;
    }

    if (!filter.Check(it->first)) {
        return;
    }

    for (auto require: it->second.requirements) {
        createModule(Module::Registry().find(require), filter);
    }

    auto &&module = it->second.create();
    m_modules[it->first] = std::move(module);
    m_moduleStages[it->second.stage].emplace_back(it->first);
}

void Engine::destroyModule(TypeId id) {
    if (!m_modules.contains(id)) {
        return;
    }

    for (const auto &[id, registrar]: Module::Registry()) {
        if (std::find(registrar.requirements.begin(), registrar.requirements.end(), id) != registrar.requirements.end()) {
            destroyModule(id);
        }
    }

    m_modules.erase(id);
}

void Engine::updateStage(Module::Stage stage) {
    for (auto &id : m_moduleStages[stage]) {
        m_modules[id]->Update();
    }
}

USING_ENGINE_NAMESPACE_END
