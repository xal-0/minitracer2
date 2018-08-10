#include <iostream>
#include <fstream>
#include <cstdlib>

#include "debug/elf.hpp"
#include "debug/byteio.hpp"

using namespace std;

int main(int argc, char **argv)
{
    if (argc != 2) {
        cerr << "wrong number of arguments" << endl;
        exit(1);
    }

    ifstream stream {argv[1], ios_base::in | ios_base::binary};
    elf::elf e {stream};
    elf::elf::section s = e.get_section(".interp");
}
