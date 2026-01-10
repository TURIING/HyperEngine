//
// Created by turiing on 2025/8/15.
//

#ifndef BASEDEFINE_H
#define BASEDEFINE_H

/***************************************************** Types ****************************************************/
enum class QueueType { Graphics, Transfer, Compute, Present };
enum class Filter { NEAREST, LINEAR };
enum class AddressMode { REPEAT, MIRROR, CLAMP_TO_EDGE, CLAMP_TO_BORDER };

struct QueueInfo {
    QueueType type     = QueueType::Graphics;
    float     priority = 1.0f; // 队列优先级，[0.0 - 1.0]
};

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Size {
    T width = 0;
    T height = 0;
};
using SizeUI = Size<uint32_t>;

template<typename T, typename = std::enable_if_t<std::is_arithmetic_v<T>>>
struct Point {
    T x = 0;
    T y = 0;
};
using PointUI = Point<uint32_t>;

using u32 = uint32_t;
using i32 = int32_t;
/***************************************************** Macros ****************************************************/

#define USING_ENGINE_NAMESPACE_BEGIN namespace HyperEngine {
#define USING_ENGINE_NAMESPACE_END }
#define USING_TIME_CONSUMING_DETECTION true

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

/***************************************************** Variable ****************************************************/
constexpr const char *VK_LAYER_KHRONOS_VALIDATION = "VK_LAYER_KHRONOS_validation";

#endif //BASEDEFINE_H
