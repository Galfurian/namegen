
#include <iostream>

#include "namegen/namegen.hpp"

int main(int, char *[])
{
    std::string name;
    std::string pattern = "!ssV'!i";
    unsigned long seed  = static_cast<unsigned long>(time(NULL));

    int ret = namegen::generate(name, pattern, seed);

    std::cout << "Name    : " << name << "\n";
    std::cout << "Pattern : " << name << "\n";
    std::cout << "Seed    : " << seed << "\n";
    std::cout << "Code    : " << ret << "\n";
    std::cout << "Size    : " << name.size() << "\n";
    std::cout << "Length  : " << name.length() << "\n";
    return 0;
}