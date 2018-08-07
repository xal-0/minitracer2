#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>

#include "elf.hpp"
#include "byteio.hpp"

using std::cout, std::endl, std::hex;
using std::invalid_argument;
using std::equal, std::begin, std::end;
using std::to_string;

namespace elf {

elf::elf(std::istream &s)
{
    elf_header h = read_obj<elf_header>(s);

    if (!equal(begin(h.magic), end(h.magic), elf_magic.begin()))
        throw invalid_argument("not an elf file (wrong magic)");

    if ((h.fileclass == 1 && elf_bits == 64)
        || (h.fileclass == 2 && elf_bits == 32))
        throw invalid_argument("only " 
                               + to_string(elf_bits) 
                               + " bits is supported");

    // if (())
    
    cout << "shoff: " << h.shoff << endl;
}

};
