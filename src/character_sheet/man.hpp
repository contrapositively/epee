#pragma once

#include <string>
#include <unordered_map>

namespace csheet
{
    class sheet_manager
    {
        public:
            static const std::string PATH;
        public:
            typedef unsigned int sheet_id;
            typedef struct {
                std::string sheet_name;
            } sheet_t;
            typedef std::unordered_map<sheet_id, sheet_t> sheet_list_t;
        public:
            static const std::unordered_map<sheet_id, sheet_t> &get_sheet_list();
    };
}
