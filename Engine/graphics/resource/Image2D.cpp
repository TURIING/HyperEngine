//
// Created by turiing on 2025/9/29.
//

#include "Image2D.h"
#include "../../resource/ResourceManager.hpp"

USING_ENGINE_NAMESPACE_BEGIN

std::shared_ptr<Image2D> Image2D::Create(const Node &node) {
    if (auto resource = ResourceManager::Instance()->Find<Image2D>(node)) {
        return resource;
    }


}

USING_ENGINE_NAMESPACE_END