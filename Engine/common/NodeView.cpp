//
// Created by turiing on 2025/10/18.
//

#include "NodeView.hpp"

USING_ENGINE_NAMESPACE_BEGIN
NodeView::NodeView(Node *parent, Key key, Node *value): NodeConstView(parent, std::move(key), value) {}

NodeView::NodeView(NodeView *parent, Key key): NodeConstView(parent, std::move(key)) {}

Node * NodeView::Get() {
    if (!HasValue()) {
        for (const auto &key: m_keys) {
            if (const auto name = std::get_if<std::string>(&key)) {
                m_pValue = &const_cast<Node*>(m_pParent)->AddProperty(*name);
            }
            else if (const auto index = std::get_if<std::uint32_t>(&key)) {
                m_pValue = &const_cast<Node*>(m_pParent)->AddProperty(*index);
            }
            else {
                LOG_CRITICAL("Key for node return is neither a int or a string");
            }
            m_pParent = m_pValue;
        }

        m_keys.erase(m_keys.begin(), m_keys.end() - 1);
    }

    return const_cast<Node *>(m_pValue);
}

NodeView NodeView::GetPropertyWithBackup(const std::string &key, const std::string &backupKey) {
    if (!HasValue()) {
        return { this, key };
    }
    return const_cast<Node*>(m_pValue)->GetPropertyWithBackup(key, backupKey);
}

NodeView NodeView::GetPropertyWithValue(const std::string &key, const NodeValue &propertyValue) {
    if (!HasValue()) {
        return { this, key };
    }
    return const_cast<Node*>(m_pValue)->GetPropertyWithValue(key, propertyValue);
}

USING_ENGINE_NAMESPACE_END
