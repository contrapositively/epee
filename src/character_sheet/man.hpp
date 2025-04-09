#pragma once

#include <string>
#include <unordered_map>

namespace csheet
{
    class sheet_manager
    {
        public:
            typedef unsigned int sheet_id;
            typedef struct {
                std::string sheet_name, game_name;
            } sheet_t;
            const std::unordered_map<sheet_id, sheet_t> &get_sheet_list() const;
    };
}
