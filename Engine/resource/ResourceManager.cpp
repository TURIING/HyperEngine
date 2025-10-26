//
// Created by turiing on 2025/9/27.
//

#include "ResourceManager.hpp"

USING_ENGINE_NAMESPACE_BEGIN
ResourceManager::ResourceManager(): m_elapsedPurge(5s) {
    LOG_INFO("ResourceManager Module Created.");
}

void ResourceManager::Update() {
    if (m_elapsedPurge.GetElapsed() != 0) {
        LOG_INFO("ResourceManager check the resource lifecycle.");
        for (auto it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it) {
            for (auto itInner = it->second.begin(); itInner != it->second.end(); ++itInner) {
                if ((*itInner).second.use_count() <= 1) {
                    it->second.erase(itInner);
                }
            }

            if (it->second.empty()) {
                m_resourceMap.erase(it);
            }
        }
    }
}

std::shared_ptr<Resource> ResourceManager::Find(const std::type_index &typeIndex, const Node &node) const {
    if (!m_resourceMap.contains(typeIndex)) {
        return nullptr;
    }

    for (const auto &[key, resource]: m_resourceMap.at(typeIndex)) {
        if (key == node) {
            return resource;
        }
    }

    return nullptr;
}

void ResourceManager::Add(const Node &node, const std::shared_ptr<Resource> &resource) {
    if (Find(resource->GetTypeIndex(), node)) {
        return;
    }

    m_resourceMap[resource->GetTypeIndex()].emplace(node, resource);
}

void ResourceManager::Remove(const std::shared_ptr<Resource> &resource) {
    auto &resources = m_resourceMap[resource->GetTypeIndex()];
    for (auto it = resources.begin(); it != resources.end(); ++it) {
        if (it->second == resource) {
            resources.erase(it);
        }
    }

    if (resources.empty()) {
        m_resourceMap.erase(resource->GetTypeIndex());
    }
}

USING_ENGINE_NAMESPACE_END
