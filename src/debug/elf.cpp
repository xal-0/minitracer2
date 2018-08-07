#include <istream>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

#include "elf.hpp"
#include "byteio.hpp"

using namespace std;

namespace elf {

elf::elf(std::istream &s)
{
    s.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);

    elf_header h = read_obj<elf_header>(s);

    if (!equal(begin(h.magic), end(h.magic), elf_magic.begin()))
        throw invalid_argument("not an elf file (wrong magic)");

    if ((h.fileclass == 1 && elf_bits == 64)
        || (h.fileclass == 2 && elf_bits == 32))
        throw invalid_argument("only " 
                               + to_string(elf_bits) 
                               + " bits is supported");

    if (h.data != 1)
        throw invalid_argument("only native byte ordering is supported\n");

    s.seekg(h.shoff + h.shentsize*h.shstrndx);

    elf_sheader shstr = read_obj<elf_sheader>(s);
    s.seekg(shstr.offset);

    vector<char> raw_shstrtab;
    raw_shstrtab.reserve(shstr.size);
    copy_n(istream_iterator<char>(s), shstr.size, raw_shstrtab.begin());

    s.seekg(h.shoff);

    for (int i = 0; i < h.shnum; i++) {
        elf_sheader sec = read_obj<elf_sheader>(s);
        string name{&raw_shstrtab[sec.name]};
        sections[name] = { sec.offset, sec.size };
    }
}

};
