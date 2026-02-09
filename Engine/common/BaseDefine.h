//
// Created by turiing on 2025/8/15.
//

#ifndef BASEDEFINE_H
#define BASEDEFINE_H
#include "Color.h"

/***************************************************** Macros ****************************************************/
#define USING_ENGINE_NAMESPACE_BEGIN namespace HyperEngine {
#define USING_ENGINE_NAMESPACE_END }
#define USING_TIME_CONSUMING_DETECTION true
#define CASE_FROM_TO(FROM, TO) case FROM: return TO;
#define ENUM_FLAG_OPERATORS(T)                                                                                                                                              \
inline T operator~ (T a) { return static_cast<T>( ~static_cast<std::underlying_type<T>::type>(a) ); }                                                                       \
inline T operator| (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) | static_cast<std::underlying_type<T>::type>(b) ); }                   \
inline T operator& (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) & static_cast<std::underlying_type<T>::type>(b) ); }                   \
inline T operator^ (T a, T b) { return static_cast<T>( static_cast<std::underlying_type<T>::type>(a) ^ static_cast<std::underlying_type<T>::type>(b) ); }                   \
inline T& operator|= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) |= static_cast<std::underlying_type<T>::type>(b) ); }   \
inline T& operator&= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) &= static_cast<std::underlying_type<T>::type>(b) ); }   \
inline T& operator^= (T& a, T b) { return reinterpret_cast<T&>( reinterpret_cast<std::underlying_type<T>::type&>(a) ^= static_cast<std::underlying_type<T>::type>(b) ); }

template<typename Enum>
constexpr auto to_underlying(Enum e) noexcept {
    return static_cast<std::underlying_type_t<Enum>>(e);
}

template<typename Enum>
constexpr bool has_flag(Enum value, Enum flag) noexcept {
    return (to_underlying(value) & to_underlying(flag)) != 0;
}

template <typename T>
uint32_t TO_U32(T value) {
    static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "T must be numeric or enum");
    return static_cast<uint32_t>(value);
}

template <typename T>
int32_t TO_I32(T value) {
    static_assert(std::is_arithmetic_v<T> || std::is_enum_v<T>, "T must be numeric or enum");
    return static_cast<int32_t>(value);
}

template <typename T>
int32_t TO_F32(T value) {
    static_assert(std::is_arithmetic_v<T>, "T must be numeric");
    return static_cast<float>(value);
}

inline void HASH_COMBINE(size_t &seed, size_t hash) {
    hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
    seed ^= hash;
}

template <class T>
inline void HASH_COMBINE(size_t &seed, const T &v) {
    std::hash<T> hasher;
    HASH_COMBINE(seed, hasher(v));
}

#define NODISCARD [[nodiscard]]
#define TODO assert(false);

namespace HyperEngine {
/***************************************************** Types ****************************************************/
enum class QueueType { Graphics, Transfer, Compute, Present };
enum class Filter { NEAREST, LINEAR };
enum class AddressMode { REPEAT, MIRROR, CLAMP_TO_EDGE, CLAMP_TO_BORDER };
enum class PrimitiveType { POINT, LINE, LINE_STRIP, TRIANGLE, TRIANGLE_STRIP };
enum class PolygonMode { FILL, LINE, POINT };
enum class CullMode { NONE, FRONT, BACK };
enum class FrontFace { COUNTER_CLOCK_WISE, CLOCK_WISE };
enum class PixelFormat {
    R8G8B8A8_SRGB,
    B8G8R8A8_SRGB,
    R16G16B16A16_SFLOAT,
};
enum class ColorComponentFlags {
    R = 1 << 0,
    G = 1 << 1,
    B = 1 << 2,
    A = 1 << 3,
};
ENUM_FLAG_OPERATORS(ColorComponentFlags);
enum class SampleCountFlags {
    SAMPLE_COUNT_1_BIT = 1 << 0,
    SAMPLE_COUNT_2_BIT = 1 << 1,
    SAMPLE_COUNT_4_BIT = 1 << 2,
    SAMPLE_COUNT_8_BIT = 1 << 3,
};

enum class CompareOp {
    NEVER,
    LESS,
    EQUAL,
    LESS_OR_EQUAL,
    GREATER,
    NOT_EQUAL,
    GREATER_OR_EQUAL,
    ALWAYS,
};
enum class ImageUsageFlags {
    TRANS_SRC = 1,
    TRANS_DST = 2,
    SAMPLED = 4,
    STORAGE = 8,
    COLOR_ATTACHMENT = 16,
    DEPTH_STENCIL_ATTACHMENT = 32,
};
ENUM_FLAG_OPERATORS(ImageUsageFlags);
enum class WaitState { Success, Failed, Timeout };
enum class AttachmentLoadOp {
    LOAD,			// 保留之前内容
    CLEAR,			// 清除为指定值
    DONT_CARE,		// 不关心
};
enum class AttachmentStoreOp {
    STORE,			// 保留供后续使用
    DONT_CARE,		// 不保留
};
enum class AttachmentType { Image, Depth, SwapChain };

struct DepthStencil {
    float    depth   = 0.0;
    uint32_t stencil = 0;
};

union ClearValue {
    Color        color;
    DepthStencil depthStencil;
};

struct QueueInfo {
    QueueType type     = QueueType::Graphics;
    float     priority = 1.0f; // 队列优先级，[0.0 - 1.0]
};

struct RasterizationInfo {
    PrimitiveType primitiveType = PrimitiveType::TRIANGLE;
    float		  lineWidth		= 1;
    PolygonMode	  polygonMode	= PolygonMode::FILL;
    CullMode	  cullMode		= CullMode::BACK;
    FrontFace	  frontFace		= FrontFace::CLOCK_WISE;
};

struct DepthStencilInfo {
    bool enableDepthTest = false;
    bool enableDepthWrite = false;
    CompareOp depthCompareOp = CompareOp::LESS;
    bool enableStencilTest = false;
};

enum class BlendFactor {
    ZERO,
    ONE,
    SRC_COLOR,
    ONE_MINUS_SRC_COLOR,
    DST_COLOR,
    ONE_MINUS_DST_COLOR,
    SRC_ALPHA,
    ONE_MINUS_SRC_ALPHA,
    DST_ALPHA,
    ONE_MINUS_DST_ALPHA,
    CONSTANT_COLOR,
    ONE_MINUS_CONSTANT_COLOR,
    CONSTANT_ALPHA,
    ONE_MINUS_CONSTANT_ALPHA,
};
enum class BlendOp {
    ADD,
    SUBTRACT,
    REVERSE_SUBTRACT,
    MIN,
    MAX,
};

struct BlendInfo {
    bool enable = false;
    BlendFactor srcColorBlendFactor = BlendFactor::SRC_ALPHA;
    BlendFactor dstColorBlendFactor = BlendFactor::ONE_MINUS_SRC_ALPHA;
    BlendFactor srcAlphaBlendFactor = BlendFactor::ONE;
    BlendFactor dstAlphaBlendFactor = BlendFactor::ONE_MINUS_SRC_ALPHA;
    BlendOp colorBlendOp = BlendOp::ADD;
    BlendOp alphaBlendOp = BlendOp::ADD;
    Color constantColor{};
    ColorComponentFlags colorWriteMask = ColorComponentFlags::R | ColorComponentFlags::G | ColorComponentFlags::B | ColorComponentFlags::A;
};

struct Attachment {
    uint32_t binding = 0;
    std::string_view name;
    AttachmentType type;
    PixelFormat format = PixelFormat::R8G8B8A8_SRGB;
    ClearValue clearValue = { Color::Black };
    AttachmentLoadOp loadOp = AttachmentLoadOp::CLEAR;
    AttachmentStoreOp storeOp = AttachmentStoreOp::STORE;
    SampleCountFlags samples = SampleCountFlags::SAMPLE_COUNT_1_BIT;
    BlendInfo blendInfo {};
};

struct SubpassInfo {
    uint32_t binding;
    std::vector<uint32_t> attachmentBindings;
};

struct MultiSampleInfo {
    bool enable = false;
    SampleCountFlags sampleCount = SampleCountFlags::SAMPLE_COUNT_1_BIT;
};

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Size2D {
    T width = 0;
    T height = 0;

    bool operator==(const Size2D &rhs) const {
        return width == rhs.width && height == rhs.height;
    }

    bool operator!=(const Size2D &rhs) const {
        return !operator==(rhs);
    }

    Size2D<T> operator* (const glm::vec2 &rhs) const {
        return { static_cast<T>(width * rhs.x), static_cast<T>(height * rhs.y) };
    }
};
template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Size3D {
    T width = 0;
    T height = 0;
    T depth = 0;

    bool operator==(const Size3D &rhs) const {
        return width == rhs.width && height == rhs.height && depth == rhs.depth;
    }

    bool operator!=(const Size3D &rhs) const {
        return !operator==(rhs);
    }
};
using Size2DUI = Size2D<uint32_t>;
using Size3DUI = Size3D<uint32_t>;

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Point2D {
    T x = 0;
    T y = 0;

    bool operator==(const Point2D &rhs) const {
        return x == rhs.x && y == rhs.y;
    }
    bool operator!=(const Point2D &rhs) const {
        return !operator==(rhs);
    }
};
using Point2DUI = Point2D<uint32_t>;
using Point2DI = Point2D<int32_t>;

struct Area2D {
    Point2DUI offset;
    Size2DUI  size;
};

struct Viewport {
    Size2DUI size;
    Point2DI offset;

    Viewport() = default;
    Viewport(const Size2DUI &size, const Point2DI &offset = {}) :
        size(size), offset(offset) {
    }
};

struct Scissor {
    Size2DUI size;
    Point2DI offset;

    Scissor() = default;
    Scissor(const Viewport &viewport): size(viewport.size), offset(viewport.offset) {}
    Scissor(const Size2DUI &size, const Point2DI &offset = {}) :
        size(size), offset(offset) {
    }
};

using Vec2 = glm::vec2;

using u32 = uint32_t;
using i32 = int32_t;

template <typename T>
using Share = std::shared_ptr<T>;

template <typename T>
using Unique = std::unique_ptr<T>;

/***************************************************** Variable ****************************************************/
constexpr const char *VK_LAYER_KHRONOS_VALIDATION = "VK_LAYER_KHRONOS_validation";

/**************************************************** Type Convert ***************************************************/
constexpr VkFormat gPixelFormatToVkFormat[] = {
    VK_FORMAT_R8G8B8A8_SRGB,                    // R8G8B8A8
    VK_FORMAT_B8G8R8A8_SRGB,                    // B8G8R8A8
    VK_FORMAT_R16G16B16A16_SFLOAT
};
constexpr VkBlendFactor gBlendFactorToVkBlendFactor[] = {
    VK_BLEND_FACTOR_ZERO,
    VK_BLEND_FACTOR_ONE,
    VK_BLEND_FACTOR_SRC_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_COLOR,
    VK_BLEND_FACTOR_DST_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_DST_COLOR,
    VK_BLEND_FACTOR_SRC_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
    VK_BLEND_FACTOR_DST_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_DST_ALPHA,
    VK_BLEND_FACTOR_CONSTANT_COLOR,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_COLOR,
    VK_BLEND_FACTOR_CONSTANT_ALPHA,
    VK_BLEND_FACTOR_ONE_MINUS_CONSTANT_ALPHA,
};

constexpr VkBlendOp gBlendOpToVkBlendOp[] = {
    VK_BLEND_OP_ADD,
    VK_BLEND_OP_SUBTRACT,
    VK_BLEND_OP_REVERSE_SUBTRACT,
    VK_BLEND_OP_MIN,
    VK_BLEND_OP_MAX,
};
}
#endif //BASEDEFINE_H
