//
// Created by turiing on 2025/10/26.
//

#include "Bitmap.h"
#include "../../file/FileManager.h"
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image.h"
#include "stb/stb_image_write.h"
#include <fstream>

USING_ENGINE_NAMESPACE_BEGIN
Bitmap::Bitmap(std::filesystem::path filename): m_filename(std::move(filename)) {
    Load(m_filename);
}

Bitmap::Bitmap(const SizeUI &size, uint32_t bytesPerPixel):
    m_data(std::make_unique<uint8_t[]>(size.width * size.height * bytesPerPixel)),
    m_size(size),
    m_bytesPerPixel(bytesPerPixel) {
}

Bitmap::Bitmap(std::unique_ptr<uint8_t[]> &&data, const SizeUI &size, uint32_t bytesPerPixel):
    m_data(std::move(data)),
    m_size(size),
    m_bytesPerPixel(bytesPerPixel) {

}

void Bitmap::Load(const std::filesystem::path &filename) {
    auto loaded = FileManager::ReadFile(filename);
    if (!loaded) {
        LOG_WARNING("Bitmap could not be loaded: {}", filename.c_str());
        return;
    }

    m_data = std::unique_ptr<uint8_t[]>(stbi_load_from_memory(
        reinterpret_cast<uint8_t*>(loaded->data()),
        TO_I32(loaded->size()),
        reinterpret_cast<int32_t*>(&m_size.width),
        reinterpret_cast<int32_t*>(&m_size.height),
        reinterpret_cast<int32_t*>(&m_bytesPerPixel),
        STBI_rgb_alpha
    ));
    m_bytesPerPixel = 4;
}

void Bitmap::Write(const std::filesystem::path &filename) const {
    if (const auto parent = filename.parent_path(); !parent.empty()) {
        std::filesystem::create_directories(parent);
    }

    std::ofstream os(filename, std::ios::binary | std::ios::out);
    int32_t len = 0;
    std::unique_ptr<uint8_t[]> png(stbi_write_png_to_mem(
        m_data.get(),
        TO_I32(m_size.width * m_bytesPerPixel),
        TO_I32(m_size.width),
        TO_I32(m_size.height),
        TO_I32(m_bytesPerPixel),
        &len
    ));
    os.write(reinterpret_cast<char*>(png.get()), TO_I32(len));
}

USING_ENGINE_NAMESPACE_END
