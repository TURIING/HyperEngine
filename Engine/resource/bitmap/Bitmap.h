//
// Created by turiing on 2025/10/26.
//

#ifndef BITMAP_H
#define BITMAP_H

#include "../../common/Common.h"
#include "BitmapFactory.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class Bitmap: public BitmapFactory<Bitmap> {
public:
    Bitmap() = default;
    explicit Bitmap(std::filesystem::path filename);
    explicit Bitmap(const Size2DUI &size, uint32_t bytesPerPixel = 4);
    explicit Bitmap(std::unique_ptr<uint8_t[]> &&data, const Size2DUI &size, uint32_t bytesPerPixel = 4);

    void Load(const std::filesystem::path &filename);
    void Write(const std::filesystem::path &filename) const;

    NODISCARD uint32_t GetLength() const { return m_size.width * m_size.height * m_bytesPerPixel; }
    NODISCARD Size2DUI GetSize() const { return m_size; }
    NODISCARD uint32_t GetBytesPerPixel() const { return m_bytesPerPixel; }
    NODISCARD const void* GetData() const { return m_data.get(); }

    void SetSize(const Size2DUI &size) { m_size = size; }
    void SetBytesPerPixel(uint32_t bytesPerPixel) { m_bytesPerPixel = bytesPerPixel; }

private:
    std::filesystem::path m_filename;
    std::unique_ptr<uint8_t[]> m_data;
    Size2DUI m_size;
    uint32_t m_bytesPerPixel = 0;
};

USING_ENGINE_NAMESPACE_END

#endif //BITMAP_H
