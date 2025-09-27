//
// Created by turiing on 2025/9/21.
//

#ifndef MODULE_HPP
#define MODULE_HPP

#include "../common/Common.h"
#include "../common/TypeInfo.hpp"

USING_ENGINE_NAMESPACE_BEGIN

template<typename Base>
class ModuleFactory {
public:
    class CreateInfo {
    public:
        std::function<std::unique_ptr<Base>()> create;
        typename Base::Stage stage;
        std::vector<TypeId> requirements;
    };

    using RegistryMap = std::unordered_map<TypeId, CreateInfo>;

    virtual ~ModuleFactory() = default;

    static RegistryMap &Registry() {
        static RegistryMap impl;
        return impl;
    }

    template<typename ... Args>
    class Requires {
    public:
        NODISCARD std::vector<TypeId> Get() const {
            std::vector<TypeId> requirements;
            (requirements.emplace_back(TypeInfo<Base>::template GetTypeId<Args>()), ...);
            return requirements;
        }
    };

    template<typename T>
    class Registrar: public Base {
    public:
        virtual ~Registrar() {
            if (static_cast<T*>(this) == moduleInstance) {
                moduleInstance = nullptr;
            }
        }

        static T* Instance() {
            return moduleInstance;
        }

    protected:
        template<typename ... Args>
        static bool Register(typename Base::Stage stage, Requires<Args...> &&requirements = {}) {
            ModuleFactory::Registry()[TypeInfo<Base>::template GetTypeId<T>()] = {
                []() {
                    moduleInstance = new T();
                    return std::unique_ptr<Base>(moduleInstance);
                },
                stage,
                requirements.Get()
            };
            return true;
        }

        inline static T* moduleInstance = nullptr;
    };
};

class ENGINE_EXPORT Module: public ModuleFactory<Module>, NoCopyable {
public:
    enum class Stage: uint8_t {
        Never, Always, Pre, Normal, Post, Render
    };

    using StageIndex = std::pair<Stage, TypeId>;

    virtual void Update() = 0;
};
template class ENGINE_EXPORT TypeInfo<Module>;

class ENGINE_EXPORT ModuleFilter {
public:
    ModuleFilter() {
        m_include.set();
    }

    template<typename T>
    ModuleFilter& Include() noexcept {
        m_include.set(TypeInfo<Module>::GetTypeId<T>());
        return *this;
    }

    template<typename T>
    ModuleFilter& Exclude() noexcept {
        m_include.reset(TypeInfo<Module>::GetTypeId<T>());
        return *this;
    }

    ModuleFilter& IncludeAll() noexcept {
        m_include.set();
        return *this;
    }

    ModuleFilter& ExcludeAll() noexcept {
        m_include.reset();
        return *this;
    }

    template<typename T>
    NODISCARD bool Check() const noexcept {
        return m_include.test(TypeInfo<Module>::GetTypeId<T>());
    }

    NODISCARD bool Check(TypeId typeId) const noexcept {
        return m_include.test(typeId);
    }

private:
    std::bitset<64> m_include;
};

USING_ENGINE_NAMESPACE_END
#endif //MODULE_HPP
