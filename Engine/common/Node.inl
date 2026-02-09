#pragma once

#include "Node.hpp"

USING_ENGINE_NAMESPACE_BEGIN

template<typename T, typename>
void Node::ParseString(std::string_view string) {
    T::Load(*this, string);
}

template<typename T, typename>
void Node::WriteStream(std::ostream &stream, NodeFormat::Format format) const {
    T::Write(*this, stream, format);
}

template<typename T>
void Node::Set(const T &value) {
    *this << value;
}
template<typename T>
void Node::Set(T &&value) {
    *this << value;
}

template<typename T>
T Node::Get() const {
    T value;
    *this >> value;
    return value;
}

template<typename T>
bool Node::Get(T &dst) const {
    if (!IsValid()) return false;
    *this >> dst;
    return true;
}

template<typename T>
bool Node::Get(T &&dst) const {
    if (!IsValid()) return false;
    *this >> dst;
    return true;
}

template<typename T>
Node &Node::operator=(const T &rhs) {
    Set(rhs);
    return *this;
}

inline Node &Node::operator<<(std::string string) {
    SetValue(std::move(string));
    SetType(NodeType::String);
    return *this;
}

inline const Node &Node::operator>>(char *&string) const {
    std::strcpy(string, GetValue().c_str());
    return *this;
}

inline Node &Node::operator<<(const char *string) {
    SetValue(string);
    SetType(NodeType::String);
    return *this;
}

inline Node &Node::operator<<(std::string_view string) {
    SetValue(std::string(string));
    SetType(NodeType::String);
    return *this;
}

inline const Node &Node::operator>>(std::string &string) const {
    string = GetValue();
    return *this;
}

inline const Node &Node::operator>>(std::filesystem::path &obj) const {
    obj = GetValue();
    return *this;
}

inline const Node &Node::operator<<(const std::filesystem::path &obj) {
    auto str = obj.string();
    std::replace(str.begin(), str.end(), '\\', '/');
    SetValue(str);
    SetType(NodeType::String);
    return *this;
}

template<typename T, std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, int>>
const Node &Node::operator>>(T &object) const {
    object = String::From<T>(GetValue());
    return *this;
}

template<typename T, std::enable_if_t<std::is_arithmetic_v<T> || std::is_enum_v<T>, int>>
Node &Node::operator<<(T object) {
    SetValue(String::To(object));
    SetType(std::is_floating_point_v<T> ? NodeType::Decimal : NodeType::Integer);
    return *this;
}

template<typename T>
Node & Node::operator<<(const VkExtent3D &extent) {
    AddProperty() << extent.width;
    AddProperty() << extent.height;
    AddProperty() << extent.depth;
    return *this;
}

template<typename T>
const Node & Node::operator>>(VkExtent3D &extent) const {

}

inline const Node &operator>>(const Node &node, bool &obj) {
    obj = String::From<bool>(node.GetValue());
    return node;
}

inline Node &operator<<(Node &node, bool obj) {
    node.SetValue(String::To(obj));
    node.SetType(NodeType::Boolean);
    return node;
}

template<typename T>
const Node &Node::operator>>(std::vector<T> &vector) const {
    vector.clear();
    vector.reserve(GetProperties().size());

    for (const auto &[propertyName, property]: GetProperties()) {
        property >> vector.emplace_back();
    }
    return *this;
}

template<typename T>
Node &Node::operator<<(const std::vector<T> &vector) {
    for (const auto &x : vector)
        AddProperty() << x;

    SetType(NodeType::Array);
    return *this;
}

template<typename T, typename K>
const Node &Node::operator>>(std::map<T, K> &map) const {
    map.clear();
    auto where = map.end();

    for (const auto &[propertyName, property] : GetProperties()) {
        std::pair<T, K> pair;
        pair.first = String::From<T>(propertyName);
        property >> pair.second;
        where = map.insert(where, std::move(pair));
    }

    return *this;
}

template<typename T, typename K>
Node &Node::operator<<(const std::map<T, K> &map) {
    for (const auto &pair : map)
        AddProperty(String::To(pair.first)) << pair.second;

    SetType(NodeType::Object);
    return *this;
}


template<typename T, typename K>
const Node &Node::operator>>(std::pair<T, K> &pair) const {
    GetProperty("first").Get(pair.first);
    GetProperty("second").Get(pair.second);
    return *this;
}

template<typename T, typename K>
Node &Node::operator<<(const std::pair<T, K> &pair) {
    GetProperty("first").Set(pair.first);
    GetProperty("second").Set(pair.second);
    return *this;
}

USING_ENGINE_NAMESPACE_END