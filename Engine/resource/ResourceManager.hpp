//
// Created by turiing on 2025/9/27.
//

#ifndef RESOURCEMANAGER_HPP
#define RESOURCEMANAGER_HPP

#include "../common/Common.h"
#include "../engine/Module.hpp"
#include "../common/ElapsedTime.hpp"
#include "../common/Node.hpp"
#include "Resource.h"
USING_ENGINE_NAMESPACE_BEGIN

using ResourceCollectType = std::unordered_map<std::type_index, std::map<Node, std::shared_ptr<Resource>>>;

class ENGINE_EXPORT ResourceManager : public Module::Registrar<ResourceManager> {
	inline static const bool Registered = Register(Stage::Post);
public:
    ResourceManager();

    void Update() override;

    NODISCARD std::shared_ptr<Resource> Find(const std::type_index &typeIndex, const Node &node) const;

    template<typename T>
    std::shared_ptr<T> Find(const Node &node) const {
        if (!m_resourceMap.contains(typeid(T))) {
            return nullptr;
        }

        for (const auto &[key, resource] : m_resourceMap.at(typeid(T))) {
            if (key == node) {
                return std::dynamic_pointer_cast<T>(resource);
            }
        }
        return nullptr;
    }

    void Add(const Node &node, const std::shared_ptr<Resource> &resource);

    void Remove(const std::shared_ptr<Resource> &resource);

private:
    ResourceCollectType m_resourceMap;
    ElapsedTime m_elapsedPurge;
};
USING_ENGINE_NAMESPACE_END
#endif //RESOURCEMANAGER_HPP
