#pragma once

#include <iostream>
#include <string>
#include <string_view>

#include <vector>
#include <set>
#include <unordered_map>


namespace ctfl
{
    template <typename T> constexpr void write_var(std::ostream& os, const T& value);
    template <typename T> constexpr void read_var(std::istream& is, T& value);

    template <typename T> constexpr void write_container(std::ostream& os, const T& container)
    {
        size_t size = container.size();
        write_var(os, size);
        for (size_t i = 0; i < size; ++i) write_var(os, container[i]);
    }
    template <typename T> constexpr void read_container(std::istream& is, T& container)
    {
        size_t size = 0;
        read_var(is, size);
        container.resize(size);
        for (size_t i = 0; i < size; ++i)
        {
            auto &value = container[i];
            read_var(is, value);
        };
    }
} // namespace ctfl

namespace ctfl
{
    template <typename T> constexpr void write_var(std::ostream& os, const T& value) { os.write((const char*)(&value), sizeof(T)); };
    template <typename T> constexpr void read_var(std::istream& is, T& value) { if (is.peek() == std::istream::traits_type::eof()) return; is.read((char*)(&value), sizeof(T)); };

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
    template <typename First, typename Second> constexpr void write_va(std::ostream& os, const std::pair<First, Second>& value) { write_var(os, value.first); write_var(os, value.second); };
    template <typename First, typename Second> constexpr void read_va(std::istream& is, std::pair<First, Second>& value) { read_var(is, value.first); read_var(is, value.second); };
    template <typename... Args> constexpr void write_var(std::ostream& os, const std::tuple<Args...>& value) { std::apply([&os](const auto&... args) { (write_var(os, args), ...); }, value); }
    template <typename... Args> constexpr void read_var(std::istream& is, std::tuple<Args...>& value) { std::apply([&is](auto&... args) { (read_var(is, args), ...); }, value); }
    
    template <> constexpr void write_var(std::ostream& os, const std::string& value) { write_container(os, value); };
    template <> constexpr void write_var(std::ostream& os, const std::string_view& value) { write_container(os, value); };
    template <> constexpr void read_var(std::istream& is, std::string& value) { read_container(is, value); };

    template<> constexpr void write_var(std::ostream& os, const std::vector<bool>& value)
    {
        size_t size = value.size();
        write_var(os, size); // Write the number of booleans
        size_t byte_size = (size + 7) / 8; // Calculate the number of bytes needed
        std::vector<uint8_t> byte_vector(byte_size, 0);
        for (size_t i = 0; i < size; ++i)
        {
            if (value[i])
            {
                byte_vector[i / 8] |= (1 << (i % 8));
            }
        }
        for (size_t i = 0; i < byte_size; ++i) write_var(os, byte_vector[i]); // Write the byte vector
    };
    template<> constexpr void read_var(std::istream& is, std::vector<bool>& value)
    {
        size_t size = 0;
        read_var(is, size); // Read the number of booleans
        size_t byte_size = (size + 7) / 8; // Calculate the number of bytes needed
        std::vector<uint8_t> byte_vector(byte_size, 0);
        for (size_t i = 0; i < byte_size; ++i) read_var(is, byte_vector[i]); // Read the byte vector
        value.resize(size);
        for (size_t i = 0; i < size; ++i) value[i] = (byte_vector[i / 8] >> (i % 8)) & 1;
    };
} // namespace ctfl