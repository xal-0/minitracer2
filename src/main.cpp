#include <iostream>
#include <fstream>
#include <cstdlib>

#include "debug/elf.hpp"

using std::cout, std::cerr, std::endl;
using std::ifstream;

int main(int argc, char **argv)
{
    if (argc != 2) {
        cerr << "wrong number of arguments" << endl;
        exit(1);
    }

    elf::elf e(argv[1]);
    auto b = e.get_section(".interp");
    std::istream is(&b);
    char c;

    for (int i = 0; i < 3; i++) {
        is >> c;
        cout << c << endl;

    }
}
