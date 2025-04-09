#pragma once

#include <unordered_map>
#include <string>


namespace csheet
{
    class sheet
    {
        private:
            void *pimpl;
        public: // sheet fields
            std::string &name();
            const std::string &name() const { return this->name(); };
            typedef unsigned short id_t;
            typedef int stat_t;
            enum class stat_id : id_t { armor_value, shock_tolerance };
            stat_t &stat(const stat_id id);
            const stat_t &stat(const stat_id id) const { return this->stat(id); };

            typedef struct {
                stat_t &value, &capacity;
            } resource_t;
            enum class resource_id : id_t { mana, stamina, hit_points };
            resource_t &resource(const resource_id id);
            const resource_t &resource(const resource_id id) const { return this->resource(id); };
        public:
            enum class initialization : unsigned char { blank, example };
            sheet(const initialization init_mode = initialization::example);
    }; // class sheet
} // namespace csheet
