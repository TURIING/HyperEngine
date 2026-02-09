#pragma once

#include "Node.hpp"

USING_ENGINE_NAMESPACE_BEGIN
inline NodeConstView NodeConstView::GetPropertyWithBackup(const std::string &key, const std::string &backupKey) const {
    if (!HasValue()) {
        return { this, key };
    }
    return m_pValue->GetPropertyWithBackup(key, backupKey);
}

template<typename T>
T NodeConstView::Get() const {
    if (!HasValue())
        return {};

    return m_pValue->Get<T>();
}

template<typename T>
bool NodeConstView::Get(T &dst) const {
    if (!HasValue()) return false;

    return m_pValue->Get<T>(dst);
}

template<typename T>
bool NodeConstView::Get(T &&dst) const {
    if (!HasValue()) return false;

    return m_pValue->Get<std::remove_reference_t<T>>(std::move(dst));
}

USING_ENGINE_NAMESPACE_END