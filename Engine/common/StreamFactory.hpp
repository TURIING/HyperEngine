//
// Created by turiing on 2025/9/24.
//

#ifndef STREAMFACTORY_HPP
#define STREAMFACTORY_HPP

#include "Common.h"
#include "TypeInfo.hpp"

USING_ENGINE_NAMESPACE_BEGIN

template<typename Base, class... Args>
class StreamFactory {
public:
    using Func = std::function<std::unique_ptr<Base>(Args...)>;
    using RegistryMap = std::unordered_map<std::string, Func>;

    virtual ~StreamFactory() = default;

    NODISCARD virtual TypeId GetTypeId() const { return -1; }
    NODISCARD virtual std::string GetTypeName() const { return ""; }

    static std::unique_ptr<Base> Create(const std::string &name, Args &&... args) {
        auto it = Registry().find(name);
        if (it == Registry().end()) {
            LOG_CRITICAL("Failed to create {}", name);
        }
        return it->second(std::forward<Args>(args)...);
    }

    static RegistryMap &Registry() {
        static RegistryMap impl;
        return impl;
    }

    template<typename T>
    class Registrar : public Base {
    public:
        NODISCARD TypeId GetTypeId() const override {
            return TypeInfo<Base>::template GetTypeId<T>();
        }

        NODISCARD std::string GetTypeName() const override { return m_name; }

    protected:
        NODISCARD static bool Register(const std::string &name) {
            Registrar::m_name = name;
            StreamFactory::Registry()[name] = [](Args... args) -> std::unique_ptr<Base> {
                return std::make_unique<T>(std::forward<Args>(args)...);
            };
            return true;
        }

    protected:
        static std::string m_name;
    };


};
USING_ENGINE_NAMESPACE_END
#endif //STREAMFACTORY_HPP
