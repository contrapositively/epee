#pragma once

#include <array>
#include <cstdint>
#include <numeric>
#include <random>

namespace dice
{
    class dice
    {
        public:
            typedef unsigned int count_t;
        private:
            std::array<count_t, 6> freq;
        public:
        constexpr size_t count() const {return std::accumulate(freq.begin(), freq.end(), 0);};
        constexpr size_t sum() const { size_t tot = 0; for (unsigned char i = 0; i < 6; ++i) tot += freq[i] * (i+1); return tot; }
        constexpr void roll(const unsigned int count);
    }; // class dice
} // namespace dice