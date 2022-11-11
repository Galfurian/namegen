
#include <iostream>

#include "namegen/namegen.hpp"

int main(int, char *[])
{
    unsigned long seed[] = { static_cast<unsigned long>(time(NULL)) };
    char buffer[256];
    namegen::generate(buffer, sizeof(buffer), "!ssV'!i", seed);
    std::cout << buffer << "\n";
    return 0;
}