//
// Created by turiing on 2026/1/15.
//

#ifndef HYPERENGINE_SURFACE_H
#define HYPERENGINE_SURFACE_H

#include "../../common/Common.h"
#include "../VulObject.h"

USING_ENGINE_NAMESPACE_BEGIN
class Window;

class Surface: public VulObject<VkSurfaceKHR> {
public:
    Surface(const Share<Window> &window);
    ~Surface() override;
};

USING_ENGINE_NAMESPACE_END
#endif //HYPERENGINE_SURFACE_H