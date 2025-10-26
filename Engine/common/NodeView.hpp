//
// Created by turiing on 2025/10/18.
//

#ifndef NODEVIEW_HPP
#define NODEVIEW_HPP

#include "Common.h"
#include "NodeConstView.hpp"
#include "Node.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class NodeView: public NodeConstView {
    friend class Node;
protected:
    NodeView() = default;
    NodeView(Node *parent, Key key, Node *value);
    NodeView(NodeView *parent, Key key);

public:
    NODISCARD Node *Get();

    explicit operator Node &() { return *Get(); }

    Node &operator*() { return *Get(); }
    Node *operator->() { return Get(); }

    template<typename T>
    void Set(const T &value) {
        Get()->Set<T>(value);
    }

    template<typename T>
    void Set(T &&value) {
        Get()->Set<std::remove_reference_t<T>>(std::move(value));
    }

    NODISCARD NodeView GetPropertyWithBackup(const std::string &key, const std::string &backupKey);
    NODISCARD NodeView GetPropertyWithValue(const std::string &key, const NodeValue &propertyValue);

    template<typename T>
    Node &operator=(const T &rhs) {
        return *Get() = rhs;
    }

    template<typename T>
    Node &operator=(T &&rhs) {
        return *Get() = std::move(rhs);
    }
};

USING_ENGINE_NAMESPACE_END

#endif //NODEVIEW_HPP
