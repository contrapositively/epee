#pragma once

#include <string>
#include <string_view>

namespace ctfl
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
    inline std::string str() const
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

    inline version(std::string_view str) { from_string(str); }
    constexpr version(unsigned int major, unsigned int minor, unsigned int patch)
        : major(major), minor(minor), patch(patch) {}
    constexpr version(std::tuple<unsigned int, unsigned int, unsigned int> version_tuple)
        : major(std::get<0>(version_tuple)), minor(std::get<1>(version_tuple)), patch(std::get<2>(version_tuple)) {}

    std::tuple<unsigned int, unsigned int, unsigned int> package() const { return std::make_tuple(major, minor, patch); }

    constexpr bool operator==(const version& other) const { return major == other.major && minor == other.minor && patch == other.patch; }
    constexpr bool operator!=(const version& other) const { return !(*this == other); }
    constexpr bool operator>(const version& other) const { if (major != other.major) return major > other.major; if (minor != other.minor) return minor > other.minor; return patch > other.patch; }
    constexpr bool operator<(const version& other) const { return (other > *this); }
    constexpr bool operator>=(const version& other) const { return !(*this < other); }
    constexpr bool operator<=(const version& other) const { return !(*this > other); }
}; // struct version
} // namespace ctfl