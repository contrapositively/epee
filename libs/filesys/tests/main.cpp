#include "filesys"

#include <iostream>
#include <random>
#include <string>
#include <unordered_map>
#include <vector>
#include <limits>
#include <cassert>

int main()
{
    constexpr int trial_count = 100;
    constexpr int file_count = 100;
    constexpr int trial_depth = 10; // Numbers like 100 cause issues with malloc and free
    const std::string dir = "files";
    const std::string filename = "test";
    const std::string file_extension = ".bin";

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> intdis(std::numeric_limits<int>::min(), std::numeric_limits<int>::max()); // Generates random integers between the minimum and maximum values for int
    std::uniform_int_distribution<> sizedis(0, trial_depth); // Generates random integers between 0 and 1000
    const std::string characters = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    std::uniform_int_distribution<> strdis(0, characters.size() - 1); // Generates random integers between 0 and the size of the characters string

    auto random_int = [&]()-> int {
        return intdis(gen);
    };
    auto random_string = [&]()-> std::string {
        int length = sizedis(gen);
        std::string str(length, ' ');
        for (int i = 0; i < length; ++i) {
            str[i] = characters[strdis(gen)];
        }
        return str;
    };
    auto random_vector = [&]()-> std::vector<std::string> {
        int length = sizedis(gen);
        std::vector<std::string> vec(length);
        for (int i = 0; i < length; ++i) {
            vec[i] = random_string();
        }
        return vec;
    };
    auto random_map = [&]()-> std::unordered_map<std::string, std::vector<int>> {
        int length = sizedis(gen);
        std::unordered_map<std::string, std::vector<int>> map;
        for (int i = 0; i < length; ++i) {
            map[random_string()] = std::vector<int>(sizedis(gen), intdis(gen));
        }
        return map;
    };

    typedef fsys::filereader<
        int,
        int,
        std::string,
        std::vector<std::string>,
        std::unordered_map<
            std::string,
            std::vector<int>
        >
    > reader_t;
    typedef reader_t::value_type data_t;

    // Create the directory if it doesn't exist
    if (std::filesystem::create_directory(dir)) {
        std::cout << "Directory created: " << dir << std::endl;
    } else {
        std::cout << "Directory already exists or failed to create: " << dir << std::endl;
    }


    for (int j = 0; j < trial_count; ++j)
    {
        for (int i = 0; i < file_count; ++i) {
            const std::string path = dir + "/" + filename + "_" + std::to_string(i) + file_extension;
            data_t data{
                random_int(),
                random_int(),
                random_string(),
                random_vector(),
                random_map()
            };
            { // save
                reader_t::file file(path, data);
                file.save();
            }
            { // load
                reader_t::file file(path, data);
                file.load();
                assert(file.is_loaded());
                assert(file.at<0>() == std::get<0>(data));
                assert(file.at<1>() == std::get<1>(data));
                assert(file.at<2>() == std::get<2>(data));
                assert(file.at<3>() == std::get<3>(data));
                const auto& map1 = file.at<4>();
                const auto& map2 = std::get<4>(data);
                assert(map1.size() == map2.size());
                for (const auto& [key, value] : map1) {
                    auto it = map2.find(key);
                    assert(it != map2.end());
                    assert(value == it->second);
                }
                assert(&file.at<0>() == &std::get<0>(file.data()));
                assert(&file.at<1>() == &std::get<1>(file.data()));
                assert(&file.at<2>() == &std::get<2>(file.data()));
                assert(&file.at<3>() == &std::get<3>(file.data()));
                assert(&file.at<4>() == &std::get<4>(file.data()));
            }
        }
    }
    return 0;
};