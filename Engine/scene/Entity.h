//
// Created by turiing on 2025/9/25.
//

#ifndef ENTITY_H
#define ENTITY_H

#include "../common/Common.h"
#include "../component/Component.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class ENGINE_EXPORT Entity final: NoCopyable {
public:
    Entity() = default;

    void Update();

    NODISCARD std::string GetName() const { return m_name; }
    void SetName(const std::string& name) { m_name = name; }

    NODISCARD bool IsRemoved() const { return m_removed; }
    void SetRemoved(bool removed) { m_removed = removed; }

    NODISCARD const std::vector<std::unique_ptr<Component>> &GetComponents() const { return m_vecComponents; }
    NODISCARD uint32_t GetComponentsCount() const { return m_vecComponents.size(); }

    template<typename T>
    T* GetComponent(bool allowDisabled = false) const {
        T *alternative = nullptr;
        for (const auto &component : m_vecComponents) {
            auto casted = dynamic_cast<T*>(component.get());

            if (!casted) continue;

            if (allowDisabled && !component->IsEnabled()) {
                alternative = casted;
                continue;
            }

            return casted;
        }

        return alternative;
    }

    template<typename T>
    std::vector<T*> GetComponents(bool allowDisabled = false) const {
        std::vector<T*> alternative;

        for (const auto &component : m_vecComponents) {
            auto casted = dynamic_cast<T*>(component.get());

            if (!casted) continue;

            if (allowDisabled && !component->IsEnabled()) {
                alternative.push_back(casted);
                continue;
            }

            alternative.push_back(casted);
        }

        return alternative;
    }

    void AddComponent(std::unique_ptr<Component> &&component);

    template<typename T, typename... Args>
    T* AddComponent(Args &&... args) {
        return dynamic_cast<T*>(AddComponent(std::make_unique<T>(std::forward<Args>(args)...)));
    }

    void RemoveComponent(Component *component);

    void RemoveComponent(const std::string &name);

    template<typename T>
    void RemoveComponent() {
        for (auto it = m_vecComponents.begin(); it != m_vecComponents.end();) {
            auto casted = dynamic_cast<T*>(it->get());

            if (casted) {
                (*it)->SetEntity(nullptr);
                m_vecComponents.erase(it);
            }
        }
    }

private:
    std::string m_name;
    bool m_removed = false;
    std::vector<std::unique_ptr<Component>> m_vecComponents;
};


USING_ENGINE_NAMESPACE_END
#endif //ENTITY_H
