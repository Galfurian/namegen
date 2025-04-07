#include "namegen/namegen.hpp"

#include <chrono>
#include <iomanip>
#include <iostream>

void generate_and_print(const std::string &pattern)
{
    auto seed = static_cast<namegen::seed_t>(std::chrono::high_resolution_clock::now().time_since_epoch().count());
    auto name = namegen::generate(pattern, seed);
    std::cout << "| " << std::setw(32) << std::right << pattern << " |";
    std::cout << " " << std::setw(32) << std::right << name << " |\n";
}

int main(int argc, char *argv[])
{
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <path_to_tokens_json>\n";
        std::cerr << "You can find a JSON with tokens inside the `examples/` folder.\n";
        return 1;
    }
    // Load tokens from the specified JSON file
    namegen::load_tokens_from_json(argv[1]);
    generate_and_print("!ssV'!i");
    generate_and_print("v!M");
    generate_and_print("c(dim)");
    generate_and_print("C!i");
    generate_and_print("<(C!i)|(v!M)>");
    generate_and_print("<C!i|v!M|>");
    return 0;
}