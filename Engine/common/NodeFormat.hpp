//
// Created by turiing on 2025/9/27.
//

#ifndef NODEFORMAT_HPP
#define NODEFORMAT_HPP

#include "Common.h"

USING_ENGINE_NAMESPACE_BEGIN

enum class NodeType: uint8_t {
    Object, Array, String, Boolean, Integer, Decimal, Null,
    Unknown, Token, EndOfFile
};

class ENGINE_EXPORT NodeFormat {
public:
    class Token {
    public:
        constexpr Token() = default;
        constexpr Token(NodeType type, std::string_view view): type(type), view(view) {}

        constexpr bool operator==(const Token &rhs) const {
            return type == rhs.type && view == rhs.view;
        }

        constexpr bool operator!=(const Token &rhs) const {
            return !(*this == rhs);
        }

        NodeType type = NodeType::Unknown;
        std::string_view view;
    };

    class NullableChar {
    public:
        constexpr NullableChar(char val): val(val) {}
        constexpr explicit operator const char&() const { return val; }

        friend std::ostream &operator<<(std::ostream &os, const NullableChar &rhs) {
            if (rhs.val != '\0') {
                os << rhs.val;
            }
            return os;
        }

        char val;
    };

    class Format {
    public:
        constexpr Format(int8_t spacesPerIndent, NullableChar newLine, NullableChar space, bool inlineArrays)
            :spacesPerIndent(spacesPerIndent), newLine(newLine), space(space), inlineArrays(inlineArrays) {}

        NODISCARD std::string GetIndent(int8_t indent) const {
            return std::string(spacesPerIndent * indent, ' ');
        }

        int8_t spacesPerIndent;
        NullableChar newLine, space;
        bool inlineArrays;
    };

    static constexpr auto Beautified = Format(2, '\n', ' ', true);
    static constexpr auto Minified = Format(0, '\0', '\0', false);

    virtual ~NodeFormat() = default;
};
USING_ENGINE_NAMESPACE_END
#endif //NODEFORMAT_HPP
