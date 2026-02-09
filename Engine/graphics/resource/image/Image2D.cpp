//
// Created by turiing on 2025/9/29.
//

#include "Image2D.h"

#include <utility>
#include "../../../resource/ResourceManager.hpp"
#include "../../Graphics.h"
#include "../../../resource/bitmap/Bitmap.h"
#include "graphics/command/CmdManager.h"
#include "graphics/command/CommandBuffer.h"

USING_ENGINE_NAMESPACE_BEGIN
std::type_index Image2D::GetTypeIndex() const {
    return typeid(Image2D);
}

WriteDescriptorSet Image2D::GetWriteDescriptorSet(uint32_t binding, VkDescriptorType descriptorType,
    const std::optional<OffsetSize> &offsetSize) const {
    return Image::GetWriteDescriptorSet(binding, descriptorType, offsetSize);
}

Share<Image2D> Image2D::Create(const Image2DCreateInfo &image2DInfo) {
    Unique<Bitmap> bitmap;
    Size2DUI size;
    if (!image2DInfo.file.empty()) {
        bitmap = std::make_unique<Bitmap>(image2DInfo.file);
        size = bitmap->GetSize();
    }

    ImageCreateInfo info{};
    info.mipLevels = image2DInfo.mipLevel;
    info.format = gPixelFormatToVkFormat[TO_U32(image2DInfo.format)];
    info.usage = static_cast<VkImageUsageFlags>(image2DInfo.usages);
    info.extent = { size.width, size.height, 0 };
    info.samples = static_cast<VkSampleCountFlagBits>(image2DInfo.samples);
    info.filter = image2DInfo.filter;
    info.samplerAddressMode = image2DInfo.addressMode;
    info.name = image2DInfo.name;

    auto temp = std::shared_ptr<Image2D>(new Image2D(image2DInfo.file, info));

    if (bitmap) {
        Graphics::Get()->GetCmdManager()->WithSingleCmdBuffer([&](CommandBuffer *pCmd) {
            pCmd->CopyBufferToImage(bitmap->GetData(), bitmap->GetLength(), temp, { {0, 0}, size});
        });
    }
    return temp;
}

Image2D::Image2D(const std::filesystem::path &fileName, const ImageCreateInfo &info): Image(info), m_filename(fileName), m_info(info) {}

Image2D::Image2D(VkImage handle, const ImageCreateInfo &info): Image(handle, info) {}

USING_ENGINE_NAMESPACE_END
