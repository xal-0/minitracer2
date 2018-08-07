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

    ifstream i{argv[1], std::ios_base::binary};
    elf::elf e(i);
}
