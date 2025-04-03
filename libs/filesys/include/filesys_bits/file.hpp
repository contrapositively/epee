#pragma once


#include "stream.hpp"

#include <vector>
#include <string>
#include <string_view>
#include <fstream>
#include <tuple>
#include <cassert>
#include <type_traits>

namespace fsys
{
template<typename... Ts>
class filereader {
public: // public types
    typedef std::tuple<Ts...> value_type;
public: // public constants
    static constexpr size_t value_count = sizeof...(Ts);
private: // private members
    value_type default_values;
public:
    value_type &defaults() { return default_values; }


public: // file class
    class file {
    private: // private members
        const std::string filepath;
        bool loaded = false;
        value_type _data;
        const value_type default_values;

    public: // public methods
        constexpr bool is_loaded() const { return loaded; }
        constexpr void clear() { loaded = false; this->_data = default_values; }

        void load() { std::ifstream ifs(filepath, std::ios::binary); if (ifs)  fsys::read_var(ifs, this->_data);  loaded = true; }
        void save() const { std::ofstream ofs(filepath, std::ios::binary | std::ios::out); if (!ofs) { throw std::ios_base::failure("Failed to open file for writing: " + filepath); } fsys::write_var(ofs, this->_data); }
    public: // constructors
        file(const std::string& path, const value_type& defaults)
            : filepath(path), _data(defaults), default_values(defaults)
        {}

        ~file() { if (loaded) save(); };

    public: // accessors and mutators
        template<size_t index>
        constexpr auto &at() const {
            static_assert(index < value_count, "Index out of range");
            return std::get<index>(this->_data);
        }

        value_type &data() { return this->_data; }
    }; // class file



public: // public members
    file read(std::string_view path) { return file(path, default_values); }
public: // constructors
    filereader() = delete;
    filereader(const value_type& defaults)
        : default_values(defaults)
    {}

    ~filereader() = default;
}; // class filereader
} // namespace fsys