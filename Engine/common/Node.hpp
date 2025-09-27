//
// Created by turiing on 2025/9/27.
//

#ifndef NODE_HPP
#define NODE_HPP

#include "Common.h"
#include "NodeFormat.hpp"

USING_ENGINE_NAMESPACE_BEGIN

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
    void WriteStream(std::ostream &stream, NodeFormat format = NodeFormat::Minified) const {
        T::Write(*this, stream, format);
    }


};
USING_ENGINE_NAMESPACE_END
#endif //NODE_HPP
