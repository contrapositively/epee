#pragma once

#include <unordered_map>
#include <string>


namespace csheet
{
    // No data is owned by this class. It simply wraps and maps the data from a hidden file system.
    class sheet
    {
        private:
            void *pimpl; // A pointer to the file data
        public: // sheet fields
            typedef unsigned short id_t;
            static const std::string DEFAULT_TEXT_V;
            enum class text_id : id_t { name, description };
            std::string &text(const text_id id, const std::string_view &default_v = DEFAULT_TEXT_V);
            const std::string &text(const text_id id, const std::string_view &default_v = DEFAULT_TEXT_V) const { return this->text(id); };

            typedef int stat_t;
            static const stat_t DEFAULT_STAT_V;
            enum class stat_id : id_t { armor_value, shock_tolerance };
            stat_t &stat(const stat_id id, const stat_t default_v = DEFAULT_STAT_V);
            const stat_t &stat(const stat_id id, const stat_t default_v = DEFAULT_STAT_V) const { return this->stat(id); };

            typedef struct {
                stat_t value, capacity;
            } resource_t;
            static const resource_t DEFAULT_RESOURCE_V;
            enum class resource_id : id_t { mana, stamina, hit_points };
            resource_t &resource(const resource_id id, const resource_t &default_v = DEFAULT_RESOURCE_V);
            const resource_t &resource(const resource_id id, const resource_t &default_v = DEFAULT_RESOURCE_V) const { return this->resource(id); };
        public:
            enum class initialization : unsigned char { blank, example };
            sheet(void *_pimpl) : pimpl(_pimpl) {};
            sheet(const initialization init_mode = initialization::example);
            sheet(const sheet &other) : pimpl(other.pimpl) {};
            sheet(sheet &&other) : pimpl(other.pimpl) {};
    }; // class sheet

    // Owns the file data. The unordered_map interface is the user-side sheet index.
    // All data is stored and managed internally.
    // On destruction, files are written to match the user-side interface.
    // If data is found in the internal structure, then it is written.
    // Otherwise, generic data is written.
    class sheet_list : public std::unordered_map<unsigned int, sheet>
    {
        private:
            void* pimpl; // Pointer to the meta file
            void* pimpl_vec; // Pointer to the internal data structure
        public:
            std::pair<unsigned int, sheet> new_sheet();
            bool del_sheet(const unsigned int sheet_id);

            sheet_list();
            ~sheet_list();
    };
} // namespace csheet
