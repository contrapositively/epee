#include "tests.hpp"


#include <iostream>


int main()
{
    constexpr int trial_count = 100;
    constexpr int file_count = 10;
    const std::string dir = std::filesystem::path(__FILE__).stem().string();
    const std::string filename = "test";

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
            data_t data = random_data();
            { // save
                file_t cexpr_file(path, data);
                cexpr_file.save();
            }
            { // load
                file_t cexpr_file(path, data);
                cexpr_file.load();
                assert(cexpr_file.is_loaded());
                assert(cexpr_file.at<0>() == std::get<0>(data));
                assert(cexpr_file.at<1>() == std::get<1>(data));
                assert(cexpr_file.at<2>() == std::get<2>(data));
                assert(cexpr_file.at<3>() == std::get<3>(data));
                const auto& map1 = cexpr_file.at<4>();
                const auto& map2 = std::get<4>(data);
                assert(map1.size() == map2.size());
                for (const auto& [key, value] : map1) {
                    auto it = map2.find(key);
                    assert(it != map2.end());
                    assert(value == it->second);
                }
                assert(&cexpr_file.at<0>() == &std::get<0>(cexpr_file.data()));
                assert(&cexpr_file.at<1>() == &std::get<1>(cexpr_file.data()));
                assert(&cexpr_file.at<2>() == &std::get<2>(cexpr_file.data()));
                assert(&cexpr_file.at<3>() == &std::get<3>(cexpr_file.data()));
                assert(&cexpr_file.at<4>() == &std::get<4>(cexpr_file.data()));
            }
        }
    }
    return 0;
};