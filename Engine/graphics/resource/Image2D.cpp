//
// Created by turiing on 2025/9/29.
//

#include "Image2D.h"
#include "../../resource/ResourceManager.hpp"
#include "../Graphics.h"
#include "../../common/NodeConstView.hpp"
#include "../../common/NodeView.hpp"

USING_ENGINE_NAMESPACE_BEGIN

std::shared_ptr<Image2D> Image2D::Create(const Node &node) {
    if (auto resource = ResourceManager::Instance()->Find<Image2D>(node)) {
        return resource;
    }
    auto result = std::make_shared<Image2D>("");
    ResourceManager::Instance()->Add(node, std::dynamic_pointer_cast<Resource>(result));
    node >> *result;


}

Image2D::Image2D(std::filesystem::path filename, bool isMipmap, Filter filter, AddressMode addressMode)
    : m_filename(filename), m_filter(filter), m_addressMode(addressMode), m_isMipmap(isMipmap) {}

const Node &operator>>(const Node &node, Image2D &image) {
    node["filename"].Get(image.m_filename);
    node["filter"].Get(image.m_filter);
    node["addressMode"].Get(image.m_addressMode);
    node["mipmap"].Get(image.m_isMipmap);
    return node;
}

Node &operator<<(Node &node, const Image2D &image) {
    node["filename"].Set(image.m_filename);
    node["filter"].Set(image.m_filter);
    node["addressMode"].Set(image.m_addressMode);
    node["mipmap"].Set(image.m_isMipmap);
    return node;
}

USING_ENGINE_NAMESPACE_END
