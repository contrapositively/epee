#pragma once

#include <array>

namespace dice
{
    class roll_statement
    {
    private:
        unsigned int clean_dice;

        enum class ALTERATION : unsigned char
        {
            CUT,
            DROP,
            BUMP,
            MAX
        }; // enum ALTERATION
        std::array<unsigned int, 4> alt_dice;
    public:
        constexpr unsigned int &differentiability() { return clean_dice; }
        constexpr unsigned int &homogeneity() { return; };
    }; // class roll_statement
} // namespace dice