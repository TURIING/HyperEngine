//
// Created by turiing on 2026/2/4.
//

#ifndef HYPERENGINE_WINDOWS_H
#define HYPERENGINE_WINDOWS_H
#include "../common/Common.h"
#include "../engine/Module.hpp"
USING_ENGINE_NAMESPACE_BEGIN
class Window;

class Windows: public Module::Registrar<Windows> {
	inline static const bool Registered = Register(Stage::Pre);
public:
    Windows();
    ~Windows();
    void Update() override;
    void AddWindow(std::string_view name);
    rocket::signal<void(const Share<Window> &)> &OnAddWindow() { return m_sigOnAddWindow; }
    NODISCARD Share<Window>& GetWindow(std::string_view name = {});

private:
    std::unordered_map<std::string_view, Share<Window>> m_mapWindows;
    rocket::signal<void(const Share<Window> &)> m_sigOnAddWindow;
};


USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_WINDOWS_H