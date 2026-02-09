//
// Created by turiing on 2026/2/4.
//

#include "Windows.h"
#include "Window.h"
#include <glfw/glfw3.h>


USING_ENGINE_NAMESPACE_BEGIN
Windows::Windows() {
    if (glfwInit() == GLFW_FALSE)
        throw std::runtime_error("GLFW failed to initialize");

    // Checks Vulkan support on GLFW.
    if (glfwVulkanSupported() == GLFW_FALSE)
        throw std::runtime_error("GLFW failed to find Vulkan support");

    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_STENCIL_BITS, 8);
    glfwWindowHint(GLFW_STEREO, GLFW_FALSE);
    LOG_INFO("Windows Module Created.");
}

Windows::~Windows() {
    glfwTerminate();
}

void Windows::Update() {
    glfwPollEvents();
    for (auto &[name, window]: m_mapWindows) {
        window->Update();
    }
}

void Windows::AddWindow(std::string_view name) {
    m_mapWindows.insert({name, std::make_shared<Window>(name)});
    m_sigOnAddWindow(m_mapWindows[name]);
    LOG_DEBUG("Add Window({})", name);
}

Share<Window> & Windows::GetWindow(std::string_view name) {
    if (name.empty()) {
        return m_mapWindows.begin()->second;
    }

    if (const auto it = m_mapWindows.find(name); it != m_mapWindows.end()) {
        return it->second;
    }

    LOG_CRITICAL("The window corresponding to {} cannot be found", name);
}

USING_ENGINE_NAMESPACE_END
