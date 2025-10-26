//
// Created by turiing on 2025/8/7.
//

#ifndef ENGINE_H
#define ENGINE_H

#include "../common/Common.h"
#include "../common/TypeInfo.hpp"
#include "Module.hpp"
#include "App.h"
#include "../common/ElapsedTime.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class ENGINE_EXPORT Engine : NoCopyable {
public:
    NODISCARD static Engine* Instance();
    void Init(const std::string &argv0, const ModuleFilter& filter = {});
    void Run();
    void SetApp(std::unique_ptr<App> &&app) { m_pApp = std::move(app); }
    void RequestClose() { m_running = false; }
    NODISCARD std::string GetArgv0() const { return m_argv0; }

private:
    Engine() = default;
    ~Engine();
    void createModule(Module::RegistryMap::const_iterator it, const ModuleFilter& filter);
    void destroyModule(TypeId id);
    void updateStage(Module::Stage stage);

private:
    std::unordered_map<TypeId, std::unique_ptr<Module>> m_modules;
    std::unordered_map<Module::Stage, std::vector<TypeId>> m_moduleStages;
    bool m_running = true;
    std::unique_ptr<App> m_pApp;
    ElapsedTime m_elapsedRender;
    ElapsedTime m_elapsedUpdate;
    std::string m_argv0;
};

USING_ENGINE_NAMESPACE_END

#endif //ENGINE_H