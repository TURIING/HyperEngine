//
// Created by turiing on 2025/8/15.
//

#ifndef BASEDEFINE_H
#define BASEDEFINE_H

/***************************************************** Types ****************************************************/
enum class GpuType { OPENGL, VULKAN };

/***************************************************** Macros ****************************************************/

#define USING_ENGINE_NAMESPACE_BEGIN namespace HyperEngine {
#define USING_ENGINE_NAMESPACE_END }

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

#define NODISCARD [[nodiscard]]

#endif //BASEDEFINE_H
