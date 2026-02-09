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
    Window(std::string_view name);
    ~Window() override;
    void Update();
    NODISCARD Size2DUI GetSize() const { return m_size * m_scale; }
    NODISCARD std::string_view GetName() const { return m_windowName; }
    NODISCARD rocket::signal<void(glm::vec2)> &OnMouseMove() { return m_sigOnMouseMove; }
    NODISCARD rocket::signal<void(glm::vec2)> &OnMouseScroll() { return m_sigOnMouseScroll; }
    NODISCARD rocket::signal<void(glm::vec2)> &OnResize() { return m_sigOnReSize; }
    NODISCARD rocket::signal<void(glm::vec2)> &OnWindowMove() { return m_sigOnWindowMove; }
    NODISCARD rocket::signal<void()> &OnWindowClose() { return m_sigOnWindowClose; }
    NODISCARD const void *GetNativeHandle() const;
    VkResult CreateSurface(VkInstance instance, VkSurfaceKHR *surface);

private:
    friend void CallbackWindowMove(GLFWwindow *pWindow, int32_t xPos, int32_t yPos);
    friend void CallbackWindowReSize(GLFWwindow *pWindow, int32_t width, int32_t height);
    friend void CallbackWindowClose(GLFWwindow *pWindow);
    friend void CallbackCursorPos(GLFWwindow *pWindow, double xPos, double yPos);
    friend void CallbackCursorScroll(GLFWwindow *pWindow, double xOffset, double yOffset);

private:
    std::string_view m_windowName;
    GLFWwindow *m_pWindow = nullptr;
    Size2DUI m_size = { 1200, 1000 };
    glm::vec2 m_position;
    std::string m_title;
    glm::vec2 m_scale;

    glm::vec2 m_mousePos;
    glm::vec2 m_mouseLastPos;
    glm::vec2 m_mouseDeltaPos;
    glm::vec2 m_mouseScroll;
    glm::vec2 m_mouseLastScroll;
    glm::vec2 m_mouseDeltaScroll;

    bool m_resizable = true;
    bool m_floating = false;
    bool m_fullscreen = false;
    bool m_closed = false;
    bool m_focused = true;
    bool m_cursorHidden = false;

    rocket::signal<void(glm::vec2)> m_sigOnMouseMove;
    rocket::signal<void(glm::vec2)> m_sigOnMouseScroll;
    rocket::signal<void(glm::vec2)> m_sigOnReSize;
    rocket::signal<void(glm::vec2)> m_sigOnWindowMove;
    rocket::signal<void()> m_sigOnWindowClose;
};


USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_WINDOW_H