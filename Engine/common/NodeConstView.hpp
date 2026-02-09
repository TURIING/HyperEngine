//
// Created by turiing on 2025/10/8.
//

#ifndef NODECONSTVIEW_HPP
#define NODECONSTVIEW_HPP

#include "Common.h"

USING_ENGINE_NAMESPACE_BEGIN
class Node;
using NodeValue = std::string;
using NodeProperty = std::pair<std::string, Node>;

class ENGINE_EXPORT NodeConstView {
    friend class Node;
protected:
    using Key = std::variant<std::string, uint32_t>;

    NodeConstView() = default;
    NodeConstView(const Node *parent, Key key, const Node *value): m_pParent(parent), m_pValue(value), m_keys{std::move(key)} {}
    NodeConstView(const NodeConstView* parent, Key key): m_pParent(parent->m_pParent), m_keys(parent->m_keys) {
        m_keys.emplace_back(std::move(key));
    }
public:
    NODISCARD bool HasValue() const { return m_pValue != nullptr; }
    NODISCARD const Node* Get() const { return m_pValue; }
    explicit operator const Node &() const { return *m_pValue; }
    explicit operator bool() const noexcept { return HasValue(); }
    const Node& operator*() const { return *m_pValue; }
    const Node* operator->() const { return m_pValue; }

    NODISCARD NodeConstView GetPropertyWithBackup(const std::string &key, const std::string &backupKey) const;

    template<typename T>
    T Get() const;

    template<typename T>
    bool Get(T &dst) const;

    template<typename T>
    bool Get(T &&dst) const;

protected:
    const Node* m_pParent = nullptr;
    const Node* m_pValue = nullptr;
    std::vector<Key> m_keys;
};
USING_ENGINE_NAMESPACE_END
#endif //NODECONSTVIEW_HPP
