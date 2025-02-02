//------------------------------------------------------------------------------
/*
    This file is part of clio: https://github.com/XRPLF/clio
    Copyright (c) 2022, the clio developers.

    Permission to use, copy, modify, and distribute this software for any
    purpose with or without fee is hereby granted, provided that the above
    copyright notice and this permission notice appear in all copies.

    THE  SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
    WITH  REGARD  TO  THIS  SOFTWARE  INCLUDING  ALL  IMPLIED  WARRANTIES  OF
    MERCHANTABILITY  AND  FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
    ANY  SPECIAL,  DIRECT,  INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
    WHATSOEVER  RESULTING  FROM  LOSS  OF USE, DATA OR PROFITS, WHETHER IN AN
    ACTION  OF  CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
    OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/
//==============================================================================

#pragma once

#include <optional>
#include <queue>
#include <stdexcept>
#include <string>

namespace util::detail {

/**
 * @brief Thrown when a KeyPath related error occurs
 */
struct KeyException : public ::std::logic_error {
    KeyException(::std::string msg) : ::std::logic_error{msg}
    {
    }
};

/**
 * @brief Thrown when a Store (config's storage) related error occurs.
 */
struct StoreException : public ::std::logic_error {
    StoreException(::std::string msg) : ::std::logic_error{msg}
    {
    }
};

/**
 * @brief Simple string tokenizer. Used by @ref Config.
 *
 * @tparam KeyType The type of key to use
 * @tparam Separator The separator character
 */
template <typename KeyType, char Separator>
class Tokenizer final {
    using opt_key_t = std::optional<KeyType>;
    KeyType key_;
    KeyType token_{};
    std::queue<KeyType> tokens_{};

public:
    explicit Tokenizer(KeyType key) : key_{key}
    {
        if (key.empty())
            throw KeyException("Empty key");

        for (auto const& c : key) {
            if (c == Separator) {
                saveToken();
            } else {
                token_ += c;
            }
        }

        saveToken();
    }

    [[nodiscard]] opt_key_t
    next()
    {
        if (tokens_.empty())
            return std::nullopt;
        auto token = tokens_.front();
        tokens_.pop();
        return std::make_optional(std::move(token));
    }

private:
    void
    saveToken()
    {
        if (token_.empty())
            throw KeyException("Empty token in key '" + key_ + "'.");
        tokens_.push(std::move(token_));
        token_ = {};
    }
};

template <typename T>
static constexpr char const*
typeName()
{
    return typeid(T).name();
}

template <>
constexpr char const*
typeName<uint64_t>()
{
    return "uint64_t";
}

template <>
constexpr char const*
typeName<int64_t>()
{
    return "int64_t";
}

template <>
constexpr char const*
typeName<uint32_t>()
{
    return "uint32_t";
}

template <>
constexpr char const*
typeName<int32_t>()
{
    return "int32_t";
}

template <>
constexpr char const*
typeName<bool>()
{
    return "bool";
}

template <>
constexpr char const*
typeName<std::string>()
{
    return "std::string";
}

template <>
constexpr char const*
typeName<char const*>()
{
    return "const char*";
}

template <>
constexpr char const*
typeName<double>()
{
    return "double";
}

};  // namespace util::detail
