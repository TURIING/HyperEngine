//
// Created by turiing on 2026/1/10.
//

#include "Window.h"
#include <glfw/include/glfw3.h>

USING_ENGINE_NAMESPACE_BEGIN
Window::Window(std::size_t id): m_id(id) {
    // m_pWindow = glfwCreateWindow(m_size.width, m_size.height, m_title.c_str(), nullptr, nullptr);
    // if (!m_pWindow) {
    //     glfwTerminate();
    //     LOG_CRITICAL("GLFW failed to create the window");
    // }
    //
    // glfwSetWindowUserPointer(m_pWindow, this);
    // glfwSetWindowAttrib(m_pWindow, GLFW_RESIZABLE, m_resizable);
    // glfwSetWindowAttrib(m_pWindow, GLFW_FLOATING, m_floating);
    // glfwShowWindow(m_pWindow);
}

Window::~Window() {
    // glfwDestroyWindow(m_pWindow);
    m_closed = true;
}

USING_ENGINE_NAMESPACE_END
