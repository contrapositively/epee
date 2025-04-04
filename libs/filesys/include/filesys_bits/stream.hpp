#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include <vector>
#include <set>
#include <unordered_map>


namespace fsys
{
    template <typename T> constexpr void write_var(std::ostream& os, const T& value) { os.write(reinterpret_cast<const char*>(&value), sizeof(T)); };
    template <typename T> constexpr void read_var(std::istream& is, T& value) { if (is.peek() == std::istream::traits_type::eof()) return; is.read(reinterpret_cast<char*>(&value), sizeof(T)); };

    template <typename T> constexpr void write_container(std::ostream& os, const T& container)
    {
        size_t size = container.size();
        write_var(os, size);
        for (const auto& item : container)
        {
            write_var(os, item);
        }
    }
    template <typename T> constexpr void read_container(std::istream& is, T& container)
    {
        size_t size = 0;
        read_var(is, size);
        container.resize(size);
        for (auto& item : container)
        {
            read_var(is, item);
        }
    }
} // namespace fsys

namespace fsys
{
    template <typename T> constexpr void write_var(std::ostream& os, const std::vector<T>& value) { write_container(os, value); };
    template <typename T> constexpr void read_var(std::istream& is, std::vector<T>& value) { read_container(is, value); };
    template <typename T> constexpr void write_var(std::ostream& os, const std::set<T>& value) { write_container(os, value); };
    template <typename T> constexpr void read_var(std::istream& is, std::set<T>& value) { read_container(is, value); };
    template <typename Key, typename Value> constexpr void write_var(std::ostream& os, const std::unordered_map<Key, Value>& value)
    {
        size_t size = value.size();
        write_var(os, size);
        for (const auto& [key, val] : value)
        {
            write_var(os, key);
            write_var(os, val);
        }
    };
    template <typename Key, typename Value> constexpr void read_var(std::istream& is, std::unordered_map<Key, Value>& value)
    {
        size_t size = 0;
        read_var(is, size);
        for (size_t i = 0; i < size; ++i)
        {
            Key key;
            Value val;
            read_var(is, key);
            read_var(is, val);
            value[key] = val;
        }
    };
    template <typename First, typename Second> constexpr void write_var(std::ostream& os, const std::pair<First, Second>& value) { write_var(os, value.first); write_var(os, value.second); };
    template <typename First, typename Second> constexpr void read_var(std::istream& is, std::pair<First, Second>& value) { read_var(is, value.first); read_var(is, value.second); };
    template <typename... Args> constexpr void write_var(std::ostream& os, const std::tuple<Args...>& value) { std::apply([&os](const auto&... args) { (write_var(os, args), ...); }, value); }
    template <typename... Args> constexpr void read_var(std::istream& is, std::tuple<Args...>& value) { std::apply([&is](auto&... args) { (read_var(is, args), ...); }, value); }
    
    constexpr void write_var(std::ostream& os, const std::string& value) { write_container(os, value); };
    constexpr void write_var(std::ostream& os, const std::string_view& value) { write_container(os, value); };
    constexpr void read_var(std::istream& is, std::string& value) { read_container(is, value); };
} // namespace fsys