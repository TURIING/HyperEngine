#pragma once

#include "Node.hpp"

USING_ENGINE_NAMESPACE_BEGIN

template<typename T>
void NodeView::Set(const T &value) {
    Get()->Set<T>(value);
}

template<typename T>
void NodeView::Set(T &&value) {
    Get()->Set<std::remove_reference_t<T>>(std::move(value));
}

template<typename T>
Node &NodeView::operator=(const T &rhs) {
    return *Get() = rhs;
}

template<typename T>
Node &NodeView::operator=(T &&rhs) {
    return *Get() = std::move(rhs);
}

USING_ENGINE_NAMESPACE_END