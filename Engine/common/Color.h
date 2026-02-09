//
// Created by turiing on 2026/1/15.
//

#ifndef HYPERENGINE_COLOR_H
#define HYPERENGINE_COLOR_H
#include <stdexcept>

namespace HyperEngine {
class Color {
public:
    enum class Type { RGBA, RGB, ARGB };
    float r = 0.0;
    float g = 0.0;
    float b = 0.0;
    float a = 1.0;

    constexpr Color() = default;
    constexpr Color(float r, float g, float b, float a) : r(r), g(g), b(b), a(a) {}
    constexpr Color(uint32_t c, Type type = Type::RGBA) {
        switch (type) {
            case Type::RGBA:
                r = static_cast<float>((uint8_t)(c >> 24 & 0xFF)) / 255.0f;
                g = static_cast<float>((uint8_t)(c >> 16 & 0xFF)) / 255.0f;
                b = static_cast<float>((uint8_t)(c >> 8 & 0xFF)) / 255.0f;
                a = static_cast<float>((uint8_t)(c & 0xFF)) / 255.0f;
                break;
            case Type::ARGB:
                r = static_cast<float>((uint8_t)(c >> 16)) / 255.0f;
                g = static_cast<float>((uint8_t)(c >> 8)) / 255.0f;
                b = static_cast<float>((uint8_t)(c & 0xFF)) / 255.0f;
                a = static_cast<float>((uint8_t)(c >> 24)) / 255.0f;
                break;
            case Type::RGB:
                r = static_cast<float>((uint8_t)(c >> 16)) / 255.0f;
                g = static_cast<float>((uint8_t)(c >> 8)) / 255.0f;
                b = static_cast<float>((uint8_t)(c & 0xFF)) / 255.0f;
                a = 1.0f;
                break;
            default:
                throw std::runtime_error("Unknown Color type");
        }
    }
    static const Color Clear;
    static const Color Black;
    static const Color Grey;
    static const Color White;
    static const Color Red;
    static const Color Yellow;
    static const Color Green;
    static const Color Blue;
};
}
#endif //HYPERENGINE_COLOR_H