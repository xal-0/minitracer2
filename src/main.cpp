#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

#include "debug/byteio.hpp"
#include "debug/dwarf.hpp"
#include "debug/elf.hpp"
#include "debug/sectioned.hpp"

int main(int argc, char **argv)
{
    if (argc != 3) {
        std::cerr << "wrong number of arguments\n";
        exit(1);
    }

    std::ifstream stream {argv[1], std::ios_base::in | std::ios_base::binary};
    minitracer::elf binary {stream};
    minitracer::dwarf d {stream, binary};

    std::stringstream ss;
    ss << std::hex << argv[2];

    minitracer::uaddr addr;
    ss >> addr;

    auto m = d.get_linenum(addr);

    if (m.file->directory)
        std::cout << std::hex << m.address << "\t\t"
                  << *m.file->directory << "/"
                  << m.file->filename << ":"
                  << std::dec << m.line << "\n";
    else
        std::cout << std::hex << m.address << "\t\t"
                  << m.file->filename << ":"
                  << std::dec << m.line << "\n";
}
