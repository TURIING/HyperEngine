//
// Created by turiing on 2025/10/26.
//

#ifndef BITMAPFACTORY_HPP
#define BITMAPFACTORY_HPP

#include "../../common/Common.h"

USING_ENGINE_NAMESPACE_BEGIN

template<typename Base>
class BitmapFactory {
public:
    using LoadFunc = std::function<void(Base &, const std::filesystem::path &)>;
    using WriteFunc = std::function<void(const Base &, const std::filesystem::path &)>;
    using RegistryMap = std::unordered_map<std::string, std::pair<LoadFunc, WriteFunc>>;

    virtual ~BitmapFactory() = default;

    static RegistryMap &Registry() {
        static RegistryMap registry;
        return registry;
    }

    template<typename T>
    class Registrar {
    protected:
        template<typename ... Args>
        static bool Register(Args &&... names) {
            for (std::string &&name: {names...}) {
                BitmapFactory::Registry()[name] = std::make_pair(&T::Load, &T::Write);
            }
            return true;
        }
    };
};

USING_ENGINE_NAMESPACE_END
#endif //BITMAPFACTORY_HPP
