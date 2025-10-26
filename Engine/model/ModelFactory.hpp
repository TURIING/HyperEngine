//
// Created by turiing on 2025/10/8.
//

#ifndef MODELFACTORY_HPP
#define MODELFACTORY_HPP

#include "../common/Common.h"
#include "../common/Node.hpp"
#include "common/NodeConstView.hpp"
#include "common/NodeView.hpp"

USING_ENGINE_NAMESPACE_BEGIN
    template<typename Base>
class ModelFactory {
public:
    using CreateFuncNode = std::function<std::shared_ptr<Base>(const Node &)>;
    using CreateFuncFileName = std::function<std::shared_ptr<Base>(const std::filesystem::path &)>;
    using RegistryMapNode = std::unordered_map<std::string, CreateFuncNode>;
    using RegistryMapFileName = std::unordered_map<std::string, CreateFuncFileName>;

    virtual ~ModelFactory() = default;

    static std::shared_ptr<Base> Create(const Node& node) {
        auto typeName = node["type"].Get<std::string>();
        auto it = RegistryNode().find(typeName);
        return it == RegistryNode().end() ? nullptr : it->second(node);
    }

    static std::shared_ptr<Base> Create(const std::filesystem::path &fileName) {
        auto ext = fileName.extension().string();
        auto it = RegistryFileName().find(ext);
        return it == RegistryFileName().end() ? nullptr : it->second(fileName);
    }

    static RegistryMapNode &RegistryNode() {
        static RegistryMapNode impl;
        return impl;
    }

    static RegistryMapFileName &RegistryFileName() {
        static RegistryMapFileName impl;
        return impl;
    }

    template<typename T>
    class Registrar: public Base {
    protected:
        template<int Dummy = 0>
        static bool Register(const std::string &typeName) {
            name = typeName;
            ModelFactory::RegistryNode()[typeName] = [](const Node &node) -> std::shared_ptr<Base> {
                return T::Create(node);
            };
            return true;
        }

        template<int Dummy = 0>
        static bool Register(const std::string &typeName, const std::string &extension) {
            Register(typeName);
            ModelFactory::RegistryFileName()[extension] = [](const std::filesystem::path &fileName) -> std::shared_ptr<Base> {
                return T::Create(fileName);
            };
            return true;
        }

        const Node &Load(const Node &node) override {
            return node >> *dynamic_cast<T*>(this);
        }

        Node &Write(Node &node) const override {
            node["type"].Set(std::string("ddd"));
            return node << *dynamic_cast<const T*>(this);
        }

        inline static std::string name;
    };

    friend const Node &operator>>(const Node &node, Base &base) {
        return base.Load(node);
    }

    friend Node &operator<<(Node &node, const Base &base) {
        return base.Write(node);
    }

protected:
    virtual const Node &Load(const Node &node) { return node; }
    virtual Node &Write(Node &node) const { return node; }
};
USING_ENGINE_NAMESPACE_END
#endif //MODELFACTORY_HPP
