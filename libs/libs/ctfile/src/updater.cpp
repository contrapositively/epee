#include "updater.hpp"

using namespace ctfl;

std::unordered_map<size_t, size_t> updater::get_mapping(const version &old_version, const version &new_version)
{
    if (old_version == new_version) return std::unordered_map<size_t, size_t>(); // Versions are the same, trivial mapping

    std::unordered_map<size_t, size_t> data_source; // Maps new file data index to old file data index
    if (new_version > old_version) // Update
    {
        for (const auto& change : changes)
        {
            if (change.vsn > new_version) break; // All changes made before the new version have been applied
            if (change.vsn <= old_version) continue; // This change predates the old version

            // Update the data transfer map with the new indices
            for (const auto& [new_index, old_index] : change.data_transfer)
            {
                if (data_source.find(old_index) != data_source.end()) {
                    data_source[new_index] = data_source[old_index];
                    data_source.erase(old_index);
                } else {
                    data_source[new_index] = old_index;
                }
            }
        }
    }
    if (new_version < old_version) // Backdate
    {
        for (auto it = changes.end()-1; it != changes.begin()-1; --it)
        {
            const auto& change = *it;
            if (change.vsn < new_version) break; // All changes made after the new version have been applied
            if (change.vsn >= old_version) continue; // This change postdates the old version

            // Update the data transfer map with the new indices
            for (const auto& [old_index, new_index] : change.data_transfer)
            {
                if (data_source.find(old_index) != data_source.end()) {
                    data_source[new_index] = data_source[old_index];
                    data_source.erase(old_index);
                } else {
                    data_source[new_index] = old_index;
                }
            }
        }
    }

    return data_source;
}

updater::updater(const std::string_view & path)
{
    file_t file(path);
    file.load();
    const auto data = file.data();
    for (size_t i = 0; i < data.size(); ++i)
    {
        changes.push_back({ data[i].first, data[i].second });
    }
}

ctfl::updater::updater(const file_t::data_t &data)
    : changes(std::get<0>(data).size())
{
    for (const auto& change : std::get<0>(data))
    {
        changes.push_back({ change.first, change.second });
    }
}

updater::file_t::data_t updater::package() const
{
    file_t::data_t data;
    auto &vec = std::get<0>(data);
    vec.reserve(changes.size());
    for (const auto& change : changes)
    {
        vec.emplace_back(change.vsn.package(), change.data_transfer);
    }
    return data;
}