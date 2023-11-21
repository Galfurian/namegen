#include "namegen/namegen.hpp"

#include <cstdint>
#include <iostream>

int main(int, char *[])
{
    std::string name;
    std::string pattern = "!ssV'!i";
    uint64_t seed       = static_cast<uint64_t>(time(NULL));

    int ret = namegen::generate(name, pattern, seed);

    std::cout << "Name    : " << name << "\n";
    std::cout << "Pattern : " << name << "\n";
    std::cout << "Seed    : " << seed << "\n";
    std::cout << "Code    : " << ret << "\n";
    std::cout << "Size    : " << name.size() << "\n";
    std::cout << "Length  : " << name.length() << "\n";
    return 0;
}