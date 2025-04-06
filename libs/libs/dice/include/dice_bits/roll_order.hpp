#pragma once

#include <array>

namespace dice
{
    struct roll_order
    {
        unsigned int clean_dice;
        unsigned int cut_dice;
        unsigned int drop_dice;

        constexpr unsigned int differentiability() const { return clean_dice; }
        constexpr unsigned int homogeneity() const { return cut_dice + drop_dice; };
        constexpr unsigned int total_roll() const { return differentiability() + homogeneity(); }
        constexpr unsigned int max_roll() const { return differentiability() * 6; }
        constexpr unsigned int min_roll() const { return differentiability(); }
    }; // class roll_statement
} // namespace dice