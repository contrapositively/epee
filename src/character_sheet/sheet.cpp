#include "sheet.hpp"

#include <frozen/unordered_map.h>

#include <ctfile>

using namespace csheet;

const std::string PATH = "sheets";
const std::string SHEET_META_FILE = PATH + ".meta";
std::string make_filepath(unsigned int sheet_id) { return PATH + std::to_string(sheet_id) + ".sheet"; }

typedef ctfl::cexpr_file<std::unordered_map<unsigned int, std::string>> sheet_list_pimpl_t;
typedef sheet_list_pimpl_t::data_t sheet_list_data_t;
inline sheet_list_pimpl_t *sl_ptr(void *pimpl) { return reinterpret_cast<sheet_list_pimpl_t *>(pimpl); }
inline sheet_list_pimpl_t &sl_ref(void *pimpl) { return (*sl_ptr(pimpl)); }
inline void *make_sheet_list_pimpl() { return reinterpret_cast<void *>(new sheet_list_pimpl_t(SHEET_META_FILE)); }

typedef ctfl::cexpr_file<
std::unordered_map<sheet::text_id, std::string>,
    std::unordered_map<sheet::stat_id, sheet::stat_t>,
    std::unordered_map<sheet::resource_id, sheet::resource_t>
    > sheet_pimpl_t;

    typedef sheet_pimpl_t::data_t sheet_data_t;
    const sheet_data_t sheet_default = { // Default Character Sheet (John Doe)
        {
            {sheet::text_id::name, "John Doe"},
            {sheet::text_id::description, "A mysterious man..."}
        },
        {
            {sheet::stat_id::armor_value, 10},
            {sheet::stat_id::shock_tolerance, 1}
        },
        {
            {sheet::resource_id::hit_points, {1, 1}},
            {sheet::resource_id::mana, {0,0}},
            {sheet::resource_id::stamina, {10,10}}
        }
    };
inline sheet_pimpl_t *s_ptr(void *pimpl) { return reinterpret_cast<sheet_pimpl_t *>(pimpl); }
inline sheet_pimpl_t &s_ref(void *pimpl) { return (*s_ptr(pimpl)); }
inline void *make_sheet_pimpl(const std::string &filepath) { return reinterpret_cast<void *>(new sheet_pimpl_t(filepath, sheet_default, sheet_default)); }
inline std::unordered_map<unsigned int, sheet_pimpl_t *> *sl_vecptr(void *pimpl) { return reinterpret_cast<std::unordered_map<unsigned int, sheet_pimpl_t *> *>(pimpl); }
inline std::unordered_map<unsigned int, sheet_pimpl_t *> &sl_vecref(void *pimpl) { return (*sl_vecptr(pimpl)); }


std::pair<unsigned int, sheet> sheet_list::new_sheet()
{
    unsigned int new_index = 0;
    for (
            const auto end = sl_vecref(pimpl_vec).end();
            sl_vecref(pimpl_vec).find(new_index) == end;
            new_index++
        );
    const std::string new_filepath = make_filepath(new_index);
    sheet_pimpl_t *new_pimpl = s_ptr(make_sheet_pimpl(new_filepath));
    sl_vecref(pimpl_vec).insert({new_index, new_pimpl});
    this->insert({new_index, sheet(new_pimpl)});
    return {new_index, (*this)[new_index]};
}

bool sheet_list::del_sheet(const unsigned int sheet_id) {
    if (this->find(sheet_id) == this->end()) return false;
 this->erase(sheet_id); 
 return true;
}

sheet_list::sheet_list() : 
    std::unordered_map<unsigned int, sheet>(), // Initialize an empty map
    pimpl(make_sheet_list_pimpl()), // Open the meta file
    pimpl_vec(new std::unordered_map<unsigned int, sheet_pimpl_t *>()) // Create an empty data map
{
    sl_ref(pimpl).load(); // Load the meta file
    sl_vecref(pimpl_vec).reserve(sl_ref(pimpl).data().size()); // Reserve an amount of data in the data map equal to the size of the meta file index
    for (const auto &pair : sl_ref(pimpl).data()) // For each file in the meta file index
    {
        const unsigned int &id = pair.first; // Get the id
        const std::string &filepath = pair.second; // Get the filepath
        void *new_pimpl = make_sheet_pimpl(filepath); // Create the file object
        s_ref(new_pimpl).load(); // Load the file data
        sl_vecref(pimpl_vec).insert({id, s_ptr(new_pimpl)}); // Add the file data to the internal data map
        this->insert({id, sheet(new_pimpl)}); // Create a sheet interface to wrap the data and push it to the user-side map
    }
}
sheet_list::~sheet_list()
{
    sl_ref(pimpl).data().clear(); // Clear all values in the meta file
    for (const auto &pair : *this) // For each user-side pair
    {
        const unsigned int &id = pair.first; // Get the id
        const std::string filepath = make_filepath(id); // Get the filepath
        sl_ref(pimpl).data().insert({id, filepath}); // Add the id and filepath back to the meta file
        sheet_pimpl_t *data;
        try {
            data = sl_vecref(pimpl_vec).at(id); // Get the data(file) from the map if it is there
        } catch (const std::exception &e) {
            data = s_ptr(make_sheet_pimpl(filepath)); // Otherwise make a blank file
        }
        data->save(); // Write the file
        delete data; // Free the memory (if it was in the map, it was owned by the object. Otherwise, it was just created.)
        sl_vecref(pimpl_vec).erase(id); // Remove the entry from this map (the data was deleted, so it is cleaned up already)
    }
    sl_ref(pimpl).save(); // Write the meta file
    delete sl_ptr(pimpl); // Free the meta file's memory
    for (auto &pair : sl_vecref(pimpl_vec)) // For every remaining data entry (file data that is not in the meta file map)
    {
        delete pair.second; // Free the memory, discarding the file data.
    }
    delete sl_vecptr(pimpl_vec); // Free the vector's memory (it has been fully cleaned)
}


const sheet::resource_t sheet::DEFAULT_RESOURCE_V = { 0, 0 };
const sheet::stat_t sheet::DEFAULT_STAT_V = 0;
const std::string sheet::DEFAULT_TEXT_V = "";

sheet::sheet(const sheet::initialization initialization_mode)
{}

std::string &sheet::text(const sheet::text_id id, const std::string_view &default_v)
{
    std::unordered_map<text_id, std::string> &text_map = s_ref(pimpl).get<std::unordered_map<text_id, std::string>>();
    if (text_map.find(id) != text_map.end()) { text_map[id] = default_v; };
    return text_map[id];
}

sheet::stat_t &sheet::stat(const sheet::stat_id id, const sheet::stat_t default_v)
{
    std::unordered_map<stat_id, stat_t> &stat_map = s_ref(pimpl).get<std::unordered_map<stat_id, stat_t>>();
    if (stat_map.find(id) != stat_map.end()) { stat_map[id] = default_v; };
    return stat_map[id];
}

sheet::resource_t &sheet::resource(const sheet::resource_id id, const sheet::resource_t &default_v)
{
    std::unordered_map<resource_id, resource_t> &resource_map = s_ref(pimpl).get<std::unordered_map<resource_id, resource_t>>();
    if (resource_map.find(id) != resource_map.end()) { resource_map[id] = default_v; };
    return resource_map[id];
}
