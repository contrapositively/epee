#include "tests.hpp"

#include <iostream>

int main()
{
    const std::string dir = std::filesystem::path(__FILE__).stem().string();
}