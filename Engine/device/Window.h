//
// Created by turiing on 2026/1/10.
//

#ifndef HYPERENGINE_WINDOW_H
#define HYPERENGINE_WINDOW_H

#include "../common/Common.h"
struct GLFWwindow;

USING_ENGINE_NAMESPACE_BEGIN

class Window : NoCopyable {
public:
    Window(std::size_t id);
    ~Window() override;

private:
    std::size_t m_id = 0;
    GLFWwindow *m_pWindow = nullptr;
    SizeUI m_size = { 1200, 1000 };
    PointUI m_position;
    std::string m_title = "HyperEngine";

    bool m_resizable = true;
    bool m_floating = false;
    bool m_fullscreen = false;
    bool m_closed = false;
    bool m_focused = true;
    bool m_cursorHidden = false;
};


USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_WINDOW_H