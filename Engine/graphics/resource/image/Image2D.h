//
// Created by turiing on 2025/9/29.
//

#ifndef IMAGE2D_H
#define IMAGE2D_H

#include "../../../common/Common.h"
#include "../../../resource/Resource.h"
#include "Image.h"

USING_ENGINE_NAMESPACE_BEGIN
struct Image2DCreateInfo {
    Size2DUI size = {};
    std::string_view name = {};
    std::filesystem::path file = {};
    PixelFormat format = PixelFormat::R8G8B8A8_SRGB;
    Filter filter = Filter::LINEAR;
    AddressMode addressMode = AddressMode::REPEAT;
    u32 mipLevel = 0;
    ImageUsageFlags usages = ImageUsageFlags::SAMPLED | ImageUsageFlags::TRANS_SRC | ImageUsageFlags::TRANS_DST | ImageUsageFlags::COLOR_ATTACHMENT;
    SampleCountFlags samples = SampleCountFlags::SAMPLE_COUNT_1_BIT;
};

class Image2D : public Resource, public Image {
    friend class SwapChain;
public:
    NODISCARD std::type_index GetTypeIndex() const override;
    static Share<Image2D> Create(const Image2DCreateInfo &image2DInfo);

protected:
    Image2D(const std::filesystem::path &fileName, const ImageCreateInfo &info);
    Image2D(VkImage handle, const ImageCreateInfo &info);

private:
    std::filesystem::path m_filename;
    ImageCreateInfo m_info;
};

USING_ENGINE_NAMESPACE_END

#endif //IMAGE2D_H
