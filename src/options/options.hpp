#pragma once

#include <vector>
#include <string>
#include <string_view>

namespace opts
{
    class options
    {
        private:
            void* pimpl;
        public:
            typedef unsigned int option_id;
        public:
            struct flag_t
            {
                unsigned char count;

                constexpr bool is_set() const { return count > 0; };
            };
            struct option_t
            {
                std::string_view value;

                constexpr bool is_set() const { return !(value.empty()); };
            };
            struct option_list_t
            {
                std::vector<std::string_view> values;

                constexpr bool is_set() const { return values.size() > 0; };
            };
        public:
            // Returns true if the option was created (if it did not exist already)
            bool add_flag(const option_id id);
            flag_t &get_flag(const option_id id);

            bool add_option(const option_id id);
            option_t &get_option(const option_id id);

            bool add_option_list(const option_id id);
            option_list_t &get_option_list(const option_id id);
        public:
            // Returns true if the alias was set (if a duplicate alias did not exist already)
            bool add_long_alias(const option_id id, const std::string_view alias);
            bool add_short_alias(const option_id id, const char alias);
        public:
            options();
            ~options();
    };
}
