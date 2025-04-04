#pragma once

#include "filesys"

#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>
#include <cassert>

const std::string file_extension = ".bin";

typedef fsys::cexpr_file<
    int,
    int,
    std::string,
    std::vector<std::string>,
    std::unordered_map<
        std::string,
        std::vector<int>
    >
> file_t;
typedef file_t::data_t data_t;



inline data_t random_data()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> intdis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()); // Generates random integers between the minimum and maximum values for int
    std::uniform_int_distribution<> sizedis(0, 10); // Generates random integers between 0 and 10 (anything larger causes issues with malloc and free)
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<> strdis(0, characters.size() - 1); // Generates random integers between 0 and the size of the characters string

    const auto random_int = [&]() -> int {
        return intdis(gen);
    };
    const auto random_string = [&]() -> std::string {
        int length = sizedis(gen);
        std::string str(length, ' ');
        for (int i = 0; i < length; ++i) {
            str[i] = characters[strdis(gen)];
        }
        return str;
    };
    const auto random_vector = [&]() -> std::vector<std::string> {
        int length = sizedis(gen);
        std::vector<std::string> vec(length);
        for (int i = 0; i < length; ++i) {
            vec[i] = random_string();
        }
        return vec;
    };
    const auto random_map = [&]() -> std::unordered_map<std::string, std::vector<int>> {
        int length = sizedis(gen);
        std::unordered_map<std::string, std::vector<int>> map;
        for (int i = 0; i < length; ++i) {
            map[random_string()] = std::vector<int>(sizedis(gen), intdis(gen));
        }
        return map;
    };


    return data_t{
        random_int(),
        random_int(),
        random_string(),
        random_vector(),
        random_map()
    };
};