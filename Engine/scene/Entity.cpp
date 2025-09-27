//
// Created by turiing on 2025/9/25.
//

#include "Entity.h"

USING_ENGINE_NAMESPACE_BEGIN
void Entity::Update() {
    for (auto it = m_vecComponents.begin(); it != m_vecComponents.end(); ++it) {
        if ((*it)->IsRemoved()) {
            m_vecComponents.erase(it);
            continue;
        }

        if ((*it)->IsEnabled()) {
            if (!(*it)->m_started) {
                (*it)->Start();
                (*it)->m_started = true;
            }

            (*it)->Update();
        }
        ++it;
    }
}

void Entity::AddComponent(std::unique_ptr<Component> &&component) {
    LOG_ASSERT(component);

    component->SetEntity(this);
    m_vecComponents.push_back(std::move(component));
}

void Entity::RemoveComponent(Component *component) {
    auto it = std::remove_if(m_vecComponents.begin(), m_vecComponents.end(), [component](const auto &c) {
        return c.get() == component;
    });
    m_vecComponents.erase(it, m_vecComponents.end());
}

void Entity::RemoveComponent(const std::string &name) {
    auto it = std::remove_if(m_vecComponents.begin(), m_vecComponents.end(), [name](const auto &c) {
        return c->GetTypeName() == name;
    });
    m_vecComponents.erase(it, m_vecComponents.end());
}

USING_ENGINE_NAMESPACE_END
