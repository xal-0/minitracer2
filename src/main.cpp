#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "debug/byteio.hpp"
#include "debug/dwarf.hpp"
#include "debug/pe.hpp"
#include "debug/sectioned.hpp"

int main(int argc, char **argv)
{
    std::cout << argv[0] << "\n";

    if (argc != 2) {
        std::cerr << "wrong number of arguments\n";
        exit(1);
    }

    std::ifstream stream {argv[1], std::ios_base::in | std::ios_base::binary};
    minitracer::pe binary {stream};
    minitracer::dwarf dwarf {stream, binary};
}
