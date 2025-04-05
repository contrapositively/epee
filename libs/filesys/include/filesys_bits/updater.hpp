#pragma once

#include "version.hpp"
#include "cexpr_file.hpp"

#include <unordered_map>

namespace fsys
{
struct updater
{
    typedef cexpr_file<
        std::vector<std::pair<
            std::tuple<unsigned int, unsigned int, unsigned int>,
            std::vector<std::pair<
                size_t, size_t
            >>
        >>
    > file_t;
    
    struct change
    {
        #ifdef VERSION
        version vsn = version(VERSION);
        #else
        #ifdef VERSION_MAJOR
        version vsn = {VERSION_MAJOR, VERSION_MINOR, VERSION_PATCH};
        #else
        version vsn = {0, 0, 0};
        #endif
        #endif

        /// @brief The data transfer map. Indexed by the new file data index, and the value is the old file data index.
        std::vector<std::pair<size_t, size_t>> data_transfer;
    }; // struct change


    std::vector<change> changes;

    /// @brief Maps the old file data index to the new file data index.
    /// @param old_version 
    /// @param new_version 
    /// @return std::unordered_map<size_t, size_t>. If the mapping is trivial (corresponding members are the same), an empty map is returned.
    std::unordered_map<size_t, size_t> get_mapping(const version &old_version, const version &new_version);

    updater(const std::string_view & path);
    updater(const file_t::data_t &data);

    file_t::data_t package() const;

    void save(const std::string_view & path) { file_t file(path, package()); file.save(); }
}; // class updater
} // namespace fsys