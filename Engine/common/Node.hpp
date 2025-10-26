//
// Created by turiing on 2025/9/27.
//

#ifndef NODE_HPP
#define NODE_HPP

#include <utility>

#include "Common.h"
#include "NodeFormat.hpp"

USING_ENGINE_NAMESPACE_BEGIN

class Node;
class NodeConstView;
class NodeView;

using NodeValue = std::string;
using NodeProperty = std::pair<std::string, Node>;

class ENGINE_EXPORT Node final {
public:
    Node() = default;
    Node(const Node&) = default;
    Node(Node &&) = default;

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T*, NodeFormat*>>>
    void ParseString(std::string_view string) {
        T::Load(*this, string);
    }

    template<typename T, typename = std::enable_if_t<std::is_convertible_v<T*, NodeFormat*>>>
    void WriteStream(std::ostream &stream, NodeFormat::Format format = NodeFormat::Minified) const {
        T::Write(*this, stream, format);
    }

    NODISCARD const NodeValue& GetValue() const { return m_value; }
    void SetValue(NodeValue value) { m_value = std::move(value); }

    NODISCARD const NodeType &GetType() const { return m_type; }
    void SetType(NodeType type) { m_type = type; }

    NODISCARD bool HasProperty(const std::string &name) const;
    NODISCARD bool HasProperty(uint32_t index) const { return index < m_vecProperties.size(); }

    NODISCARD NodeConstView GetProperty(const std::string &name) const;
    NODISCARD NodeConstView GetProperty(uint32_t index) const;
    NODISCARD NodeView GetProperty(const std::string &name);
    NODISCARD NodeView GetProperty(uint32_t index);

    Node &AddProperty(const Node &node);
    Node &AddProperty(Node &&node = {});
    Node &AddProperty(const std::string &name, const Node &node);
    Node &AddProperty(const std::string &name, Node &&node = {});
    Node &AddProperty(uint32_t index, const Node &node);
    Node &AddProperty(uint32_t index, Node &&node = {});

    Node RemoveProperty(const std::string &name);
    Node RemoveProperty(const Node &node);

    NODISCARD NodeConstView GetPropertyWithBackup(const std::string &name, const std::string &backupName) const;
    NODISCARD NodeConstView GetPropertyWithValue(const std::string &name, const NodeValue &propertyValue) const;
    NODISCARD NodeView GetPropertyWithBackup(const std::string &name, const std::string &backupName);
    NODISCARD NodeView GetPropertyWithValue(const std::string &name, const NodeValue &propertyValue);

    template<typename T>
    void Set(const T &value) {
        *this << value;
    }
    template<typename T>
    void Set(T &&value) {
        *this << value;
    }

    template<typename T>
    T Get() const {
        T value;
        *this >> value;
        return value;
    }

    NodeConstView operator[](const std::string &name) const;
    NodeConstView operator[](uint32_t index) const;
    NodeView operator[](const std::string &name);
    NodeView operator[](uint32_t index);

    Node &operator=(const Node &rhs) = default;
    Node &operator=(Node &&rhs) noexcept = default;

    bool operator==(const Node &rhs) const {
        return m_value == rhs.m_value &&
            m_vecProperties.size() == rhs.m_vecProperties.size() &&
            std::equal(m_vecProperties.begin(), m_vecProperties.end(), rhs.m_vecProperties.begin(), [](const auto &left, const auto &right) {
                return left == right;
            });
    }

    bool operator!=(const Node &rhs) const {
        return !(*this == rhs);
    }

    bool operator<(const Node &rhs) const {
        if (m_value < rhs.m_value) {
            return true;
        }

        if (rhs.m_value < m_value) {
            return false;
        }

        if (m_vecProperties < rhs.m_vecProperties) {
            return true;
        }

        if (rhs.m_vecProperties < m_vecProperties) {
            return false;
        }

        return false;
    }

    template<typename T>
    Node &operator=(const T &rhs) {
        Set(rhs);
        return *this;
    }

    Node &operator<<(std::string string) {
        SetValue(std::move(string));
        SetType(NodeType::String);
        return *this;
    }

    const Node &operator>>(char *&string) const {
        std::strcpy(string, GetValue().c_str());
        return *this;
    }

    Node &operator<<(const char *string) {
        SetValue(string);
        SetType(NodeType::String);
        return *this;
    }

    Node &operator<<(std::string_view string) {
        SetValue(std::string(string));
        SetType(NodeType::String);
        return *this;
    }

    const Node &operator>>(std::string &string) const {
        string = GetValue();
        return *this;
    }

protected:
    NodeValue m_value;
    std::vector<NodeProperty> m_vecProperties;
	NodeType m_type;
};

USING_ENGINE_NAMESPACE_END
#endif //NODE_HPP
