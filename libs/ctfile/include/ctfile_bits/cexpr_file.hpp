#pragma once


#include "stream.hpp"

#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <tuple>
#include <cassert>
#include <type_traits>

namespace ctfl
{
template<typename... Ts>
class cexpr_file {
public: // public types
    typedef std::tuple<Ts...> data_t;
public: // public constants
    static constexpr size_t value_count = sizeof...(Ts);
private: // private members
    const std::string filepath;
    bool loaded = false;
    data_t _data;
    data_t default_values;
public:
    data_t &defaults() { return default_values; }

public: // public methods
    constexpr bool is_loaded() const { return loaded; }
    constexpr void clear() { loaded = false; this->_data = default_values; }

    void load() { std::ifstream ifs(filepath, std::ios::binary); if (ifs)  ctfl::read_var(ifs, this->_data);  loaded = true; }
    constexpr bool ensure_loaded() { if (!loaded) { load(); return true; } return false; }
    void save() const { std::ofstream ofs(filepath, std::ios::binary | std::ios::out); if (!ofs) { throw std::ios_base::failure("Failed to open file for writing: " + filepath); } ctfl::write_var(ofs, this->_data); }
    constexpr bool save_if_loaded() const { if (loaded) { save(); return true; } return false; }
public: // constructors
    cexpr_file(const std::string_view& path, const data_t& values = data_t(), const data_t& defaults = data_t())
        : filepath(std::string(path)), _data(values), default_values(defaults)
    {}
    cexpr_file(const cexpr_file& other) = default;
    cexpr_file(cexpr_file&& other) noexcept = default;
    cexpr_file& operator=(const cexpr_file& other) = default;
    cexpr_file& operator=(cexpr_file&& other) noexcept = default;

    ~cexpr_file() = default;

public: // accessors and mutators
    template<size_t index>
    constexpr auto &at() { return std::get<index>(this->_data); }
    template<size_t index>
    constexpr const auto &at() const {return at<index>();};
    template<typename T>
        constexpr auto & get() { return std::get<T>(this->_data); }
    template<typename T>
        constexpr const auto &get() const { return this->get<T>(); }

    constexpr auto &data() { if constexpr (value_count == 1) return std::get<0>(this->_data); else return this->_data; }
    constexpr const auto &data() const { return this->data(); }
}; // class cexpr_file
} // namespace ctfl
