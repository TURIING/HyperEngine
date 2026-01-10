//
// Created by turiing on 2025/9/29.
//

#ifndef IMAGE2D_H
#define IMAGE2D_H

#include "../../common/Common.h"
#include "../../common/Node.hpp"
#include "../../resource/Resource.h"

USING_ENGINE_NAMESPACE_BEGIN

class Image2D : public Resource {
public:
    static std::shared_ptr<Image2D> Create(const Node &node);
    NODISCARD Image2D(std::filesystem::path filename, bool isMipmap = false, Filter filter = Filter::LINEAR, AddressMode addressMode = AddressMode::REPEAT);

    friend const Node &operator>>(const Node &node, Image2D &image);
    friend Node &operator<<(Node &node, const Image2D &image);
    NODISCARD std::type_index GetTypeIndex() const override { return typeid(Image2D); };

private:
    std::filesystem::path m_filename;
    Filter m_filter;
    AddressMode m_addressMode;
    bool m_isMipmap = false;
    HyperGpu::Image2D* m_pImage = nullptr;
};

USING_ENGINE_NAMESPACE_END

#endif //IMAGE2D_H
