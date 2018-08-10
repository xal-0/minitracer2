#include <istream>
#include <iostream>
#include <stdexcept>
#include <iterator>
#include <algorithm>
#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <ios>

#include "elf.hpp"
#include "byteio.hpp"
#include "arch/arch.hpp"

using namespace std;

namespace elf {

const std::array<u8, 4> elf_magic = { 0x7f, 'E', 'L', 'F' };

STARTPACK
struct PACK elf_header {
    u8 magic[4];
    u8 fileclass;
    u8 data;
    u8 version0;
    u8 pad[9];
    u16 type;
    u16 machine;
    u32 version1;
    uaddr entry;
    uaddr phoff;
    uaddr shoff;
    u32 flags;
    u16 ehsize;
    u16 phentsize;
    u16 phnum;
    u16 shentsize;
    u16 shnum;
    u16 shstrndx;
};

struct PACK elf_sheader {
    u32 name;
    u32 type;
    uaddr flags;
    uaddr addr;
    uaddr offset;
    uaddr size;
    u32 link;
    u32 info;
    uaddr addralign;
    uaddr entsize;
};
ENDPACK

elf::elf(istream &stream)
    : stream(stream)
{
    read_sections();
}

void elf::read_sections()
{
    stream.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);

    elf_header h = read_obj<elf_header>(stream);

    if (!equal(begin(h.magic), end(h.magic), elf_magic.begin()))
        throw invalid_argument{"not an elf file (wrong magic)"};

    if ((h.fileclass == 1 && elf_bits == 64)
        || (h.fileclass == 2 && elf_bits == 32))
        throw invalid_argument{"only " 
                + to_string(elf_bits)
                + " bits is supported"};

    if (h.data != 1)
        throw invalid_argument{"only native byte ordering is supported\n"};

    stream.seekg(h.shoff + h.shentsize*h.shstrndx);

    elf_sheader shstr = read_obj<elf_sheader>(stream);
    stream.seekg(shstr.offset);

    vector<char> raw_shstrtab;
    raw_shstrtab.reserve(shstr.size);
    copy_n(istream_iterator<char>(stream), shstr.size, raw_shstrtab.begin());

    stream.seekg(h.shoff);

    for (int i = 0; i < h.shnum; i++) {
        elf_sheader sec = read_obj<elf_sheader>(stream);
        string name{&raw_shstrtab[sec.name]};
        sections[name] = { sec.offset, sec.size };
    }
}

elf::section elf::get_section(string name)
{
    return sections.at(name);
}

};
