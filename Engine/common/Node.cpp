//
// Created by turiing on 2025/10/18.
//

#include "Node.hpp"
#include "NodeConstView.hpp"
#include "NodeView.hpp"

USING_ENGINE_NAMESPACE_BEGIN
static const NodeProperty NullNode = NodeProperty("", Node() = nullptr);

bool Node::HasProperty(const std::string &name) const {
    for (const auto &[propertyName, property]: m_vecProperties) {
        if (name == propertyName) {
            return true;
        }
    }
    return false;
}

NodeConstView Node::GetProperty(const std::string &name) const {
    for (const auto &[propertyName, property]: m_vecProperties) {
        if (name == propertyName) {
            return { this, name, &property };
        }
    }
    return { this, name, nullptr };
}

NodeConstView Node::GetProperty(uint32_t index) const {
    if (index < m_vecProperties.size()) {
        return { this, index, &m_vecProperties[index].second };
    }
    return { this, index, nullptr };
}

NodeView Node::GetProperty(const std::string &name) {
    for (auto &[propertyName, property]: m_vecProperties) {
        if (propertyName == name) {
            return { this, name, &property };
        }
    }
    return { this, name, nullptr };
}

NodeView Node::GetProperty(uint32_t index) {
    if (index < m_vecProperties.size()) {
        return { this, index, &m_vecProperties[index].second };
    }
    return { this, index, nullptr };
}

Node & Node::AddProperty(const Node &node) {
    m_type = NodeType::Array;
    return m_vecProperties.emplace_back("", node).second;
}

Node & Node::AddProperty(Node &&node) {
    m_type = NodeType::Array;
    return m_vecProperties.emplace_back("", std::move(node)).second;
}

Node & Node::AddProperty(const std::string &name, const Node &node) {
    return m_vecProperties.emplace_back(name, node).second;
}

Node & Node::AddProperty(const std::string &name, Node &&node) {
    return m_vecProperties.emplace_back(name, std::move(node)).second;
}

Node & Node::AddProperty(uint32_t index, const Node &node) {
    m_type = NodeType::Array;
    m_vecProperties.resize(std::max(m_vecProperties.size(), static_cast<std::size_t>(index + 1)), NullNode);
    return m_vecProperties[index].second = node;
}

Node & Node::AddProperty(uint32_t index, Node &&node) {
    m_type = NodeType::Array;
    m_vecProperties.resize(std::max(m_vecProperties.size(), static_cast<std::size_t>(index + 1)), NullNode);
    return m_vecProperties[index].second = std::move(node);
}

Node Node::RemoveProperty(const std::string &name) {
    for (auto it = m_vecProperties.begin(); it != m_vecProperties.end();) {
        if (it->first == name) {
            auto result = std::move(it->second);
            m_vecProperties.erase(it);
            return result;
        }
        ++it;
    }
    return {};
}

Node Node::RemoveProperty(const Node &node) {
    for (auto it = m_vecProperties.begin(); it != m_vecProperties.end();) {
        if (it->second == node) {
            auto result = std::move(it->second);
            it = m_vecProperties.erase(it);
            return result;
        }
        ++it;
    }
    return {};
}

NodeConstView Node::GetPropertyWithBackup(const std::string &name, const std::string &backupName) const {
    if (auto p1 = GetProperty(name))
        return p1;
    if (auto p2 = GetProperty(backupName))
        return p2;

    return { this, name, nullptr };
}

NodeConstView Node::GetPropertyWithValue(const std::string &name, const NodeValue &propertyValue) const {
    for (const auto &[propertyName, property]: m_vecProperties) {
        if (auto p1 = property.GetProperty(name); p1->GetValue() == propertyValue) {
            return { this, name, &property };
        }
    }
    return { this, name, nullptr };
}

NodeView Node::GetPropertyWithBackup(const std::string &name, const std::string &backupName) {
    if (auto p1 = GetProperty(name))
        return p1;
    if (auto p2 = GetProperty(backupName))
        return p2;
    return { this, name, nullptr };
}

NodeView Node::GetPropertyWithValue(const std::string &name, const NodeValue &propertyValue) {
    for (auto &[propertyName, property]: m_vecProperties) {
        if (auto p1 = property.GetProperty(name); p1->GetValue() == propertyValue) {
            return { this, name, &property };
        }
    }
    return { this, name, nullptr };
}

inline const Node &operator>>(const Node &node, Node &object) {
    object = node;
    return node;
}

inline Node &operator<<(Node &node, const Node &object) {
    node = object;
    return node;
}

inline Node &operator<<(Node &node, const std::nullptr_t &object) {
    node.SetValue("");
    node.SetType(NodeType::Null);
    return node;
}


NodeConstView Node::operator[](const std::string &name) const {
    return GetProperty(name);
}

NodeConstView Node::operator[](uint32_t index) const {
    return GetProperty(index);
}

NodeView Node::operator[](const std::string &name) {
    return GetProperty(name);
}

NodeView Node::operator[](uint32_t index) {
    return GetProperty(index);
}

USING_ENGINE_NAMESPACE_END
