//
// Created by turiing on 2026/1/15.
//

#include "Surface.h"
#include "../Graphics.h"
#include ".././../device/Window.h"
#include "../device/Instance.h"

USING_ENGINE_NAMESPACE_BEGIN
Surface::Surface(const Share<Window> &window) {
    auto pInstance = Graphics::Get()->GetInstance();
    CALL_VK(window->CreateSurface(pInstance->GetHandle(), &m_pHandle));
	LOG_DEBUG("Created Surface.");
}

Surface::~Surface() {
    auto pInstance = Graphics::Get()->GetInstance();
	vkDestroySurfaceKHR(pInstance->GetHandle(), m_pHandle, nullptr);
}

USING_ENGINE_NAMESPACE_END
