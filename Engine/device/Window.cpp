//
// Created by turiing on 2026/1/10.
//

#include "Window.h"
#define GLFW_EXPOSE_NATIVE_COCOA
#include <glfw/glfw3.h>
#include <glfw/glfw3native.h>
#include "../engine/Engine.h"

USING_ENGINE_NAMESPACE_BEGIN
void CallbackWindowReSize(GLFWwindow *pWindow, int32_t width, int32_t height) {
    auto window = static_cast<Window *>(glfwGetWindowUserPointer(pWindow));
    window->m_size = { TO_U32(width), TO_U32(height) };
    window->m_sigOnReSize({ width, height });
}

void CallbackWindowClose(GLFWwindow *pWindow) {
    auto window = static_cast<Window *>(glfwGetWindowUserPointer(pWindow));
    window->m_sigOnWindowClose();
    Engine::Instance()->RequestClose();
}

void CallbackCursorPos(GLFWwindow *pWindow, double xPos, double yPos) {
    auto window = static_cast<Window *>(glfwGetWindowUserPointer(pWindow));
    window->m_mousePos = { xPos, yPos };
    window->m_sigOnMouseMove(window->m_mousePos);
}

void CallbackCursorScroll(GLFWwindow *pWindow, double xOffset, double yOffset) {
    auto window = static_cast<Window *>(glfwGetWindowUserPointer(pWindow));
    window->m_mouseScroll= { xOffset, yOffset };
    window->m_sigOnMouseScroll(window->m_mouseScroll);
}

Window::Window(std::string_view name): m_windowName(name) {
    m_pWindow = glfwCreateWindow(m_size.width, m_size.height, m_title.c_str(), nullptr, nullptr);
    if (!m_pWindow) {
        glfwTerminate();
        LOG_CRITICAL("GLFW failed to create the window");
    }

    glfwSetWindowUserPointer(m_pWindow, this);
    glfwSetWindowAttrib(m_pWindow, GLFW_RESIZABLE, m_resizable);
    glfwSetWindowAttrib(m_pWindow, GLFW_FLOATING, m_floating);
    glfwGetWindowContentScale(m_pWindow, &m_scale.x, &m_scale.y);
    glfwShowWindow(m_pWindow);

    glfwSetWindowSizeCallback(m_pWindow, CallbackWindowReSize);
    glfwSetWindowCloseCallback(m_pWindow, CallbackWindowClose);
    glfwSetCursorPosCallback(m_pWindow, CallbackCursorPos);
    glfwSetScrollCallback(m_pWindow, CallbackCursorScroll);
}

Window::~Window() {
    glfwDestroyWindow(m_pWindow);
    m_closed = true;
}

void Window::Update() {
    auto delta = Engine::Instance()->GetDeltaTime().AsSeconds<>();

    m_mouseDeltaPos = delta * (m_mouseLastPos - m_mousePos);
    m_mouseLastPos = m_mousePos;

    m_mouseDeltaScroll = delta * (m_mouseLastScroll - m_mouseScroll);
    m_mouseLastScroll = m_mouseScroll;
}

const void* Window::GetNativeHandle() const {
    return glfwGetCocoaView(m_pWindow);
}

VkResult Window::CreateSurface(VkInstance instance, VkSurfaceKHR* surface) {
 //    uint32_t glfwExtensionCount;
 //    auto glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	// std::vector<const char *> extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);
    return glfwCreateWindowSurface(instance, m_pWindow, nullptr, surface);
}

USING_ENGINE_NAMESPACE_END
