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
    
    elf::elf e(argv[1]);
    // auto s = e.get_section(".interp");
}
