//
// Created by turiing on 2025/9/25.
//

#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include "../common/Common.h"
#include "../common/StreamFactory.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class Entity;

class ENGINE_EXPORT Component : public StreamFactory<Component> {
    friend class Entity;
public:
    virtual void Start() = 0;
    virtual void Update() = 0;

    NODISCARD bool IsEnabled() const { return m_enabled; }
    void SetEnabled(bool enabled) { m_enabled = enabled; }

    NODISCARD bool IsRemoved() const { return m_removed; }
    void SetRemoved(bool removed) { m_removed = removed; }

    NODISCARD Entity* GetEntity() const { return m_pEntity; }
    void SetEntity(Entity* pEntity) { m_pEntity = pEntity; }

private:
    bool m_started = false;
    bool m_enabled = false;
    bool m_removed = false;
    Entity* m_pEntity = nullptr;
};

USING_ENGINE_NAMESPACE_END
#endif //COMPONENT_HPP
