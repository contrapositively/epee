#pragma once

#include <string>
#include <string_view>

namespace fsys
{
struct version
{
    /// @brief The major version number.
    unsigned int major;
    /// @brief The minor version number.
    unsigned int minor;
    /// @brief The patch version number.
    unsigned int patch;

    /// @brief The string representation of the version.
    std::string str() const
    {
        return std::to_string(major) + "." + std::to_string(minor) + "." + std::to_string(patch);
    }
    void from_string(std::string_view str)
    {
        size_t pos1 = str.find('.');
        size_t pos2 = str.find('.', pos1 + 1);
        major = std::stoi(std::string(str.substr(0, pos1)));
        minor = std::stoi(std::string(str.substr(pos1 + 1, pos2 - pos1 - 1)));
        patch = std::stoi(std::string(str.substr(pos2 + 1)));
    }

    version(std::string_view str) { from_string(str); }

    constexpr bool operator==(const version& other) const { return major == other.major && minor == other.minor && patch == other.patch; }
    constexpr bool operator>(const version& other) const { if (major != other.major) return major > other.major; if (minor != other.minor) return minor > other.minor; return patch > other.patch; }
}; // struct version
} // namespace fsys