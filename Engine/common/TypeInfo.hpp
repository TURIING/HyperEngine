//
// Created by turiing on 2025/9/21.
//

#ifndef TYPEINFO_HPP
#define TYPEINFO_HPP

#include "Common.h"

USING_ENGINE_NAMESPACE_BEGIN

using TypeId = std::size_t;

template<typename T>
class TypeInfo {
public:
    TypeInfo() = delete;

    template<typename K, typename = std::enable_if_t<std::is_convertible_v<K*, T*>>>
    static TypeId GetTypeId() noexcept {
        std::type_index typeIndex(typeid(K));
        if (auto it = typeIdMap.find(typeIndex); it != typeIdMap.end()) {
            return it->second;
        }

        const auto id = NextTypeId();
        typeIdMap[typeIndex] = id;
        return id;
    }

private:
    static TypeId NextTypeId() noexcept {
        const auto id = nextTypeId;
        nextTypeId++;
        return id;
    }

private:
    static TypeId nextTypeId;
    static std::unordered_map<std::type_index, TypeId> typeIdMap;
};

template<typename K>
TypeId TypeInfo<K>::nextTypeId = 0;

template<typename K>
std::unordered_map<std::type_index, TypeId> TypeInfo<K>::typeIdMap = {};

USING_ENGINE_NAMESPACE_END
#endif //TYPEINFO_HPP
