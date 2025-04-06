#pragma once

#include "roll_order.hpp"

#include <array>
#include <cstdint>
#include <numeric>
#include <random>
#include <cassert>

namespace dice
{
    class dice_pool
    {
        public:
            typedef unsigned int count_t;
        private:
            std::array<count_t, 6> freq_tab;

            static std::mt19937 gen;
            static std::uniform_int_distribution<> dis;
            static bool is_init;
        public:
            constexpr size_t count() const {return std::accumulate(freq_tab.begin(), freq_tab.end(), 0);};
            constexpr size_t sum() const { size_t tot = 0; for (unsigned char i = 0; i < 6; ++i) tot += freq_tab[i] * (i+1); return tot; }
            void roll(const unsigned int count);
            void execute(const roll_order& order);

            constexpr count_t &freq(const unsigned char i) { assert(i>=1 && i<=6); return freq_tab[i-1]; }
            constexpr const count_t &freq(const unsigned char i) const { return freq(i); }
            constexpr count_t &operator[](const unsigned char i) { assert(i>=0 && i<=5); return freq_tab[i]; }
            constexpr const count_t &operator[](const unsigned char i) const { return (*this)[i]; }
        public:
            dice_pool();
            dice_pool(const dice_pool& other) = default;
            dice_pool& operator=(const dice_pool& other) = default;
            dice_pool(dice_pool&& other) noexcept = default;
            dice_pool& operator=(dice_pool&& other) noexcept = default;
    }; // class dice
} // namespace dice