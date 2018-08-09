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

const std::array<uint8_t, 4> elf_magic = { 0x7f, 'E', 'L', 'F' };

STARTPACK
struct PACK elf_header {
    uint8_t magic[4];
    uint8_t fileclass;
    uint8_t data;
    uint8_t version0;
    uint8_t pad[9];
    uint16_t type;
    uint16_t machine;
    uint32_t version1;
    uintptr_t entry;
    uintptr_t phoff;
    uintptr_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
};

struct PACK elf_sheader {
    uint32_t name;
    uint32_t type;
    uintptr_t flags;
    uintptr_t addr;
    uintptr_t offset;
    uintptr_t size;
    uint32_t link;
    uint32_t info;
    uintptr_t addralign;
    uintptr_t entsize;
};
ENDPACK

elf::elf(string filename)
    : stream(make_shared<ifstream>(filename, std::ios_base::binary))
{
    read_sections();
}

elf::elf(shared_ptr<istream> stream)
    : stream(stream)
{
    read_sections();
}

void elf::read_sections()
{
    stream->exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);

    elf_header h = read_obj<elf_header>(*stream);

    if (!equal(begin(h.magic), end(h.magic), elf_magic.begin()))
        throw invalid_argument{"not an elf file (wrong magic)"};

    if ((h.fileclass == 1 && elf_bits == 64)
        || (h.fileclass == 2 && elf_bits == 32))
        throw invalid_argument{"only " 
                + to_string(elf_bits)
                + " bits is supported"};

    if (h.data != 1)
        throw invalid_argument{"only native byte ordering is supported\n"};

    stream->seekg(h.shoff + h.shentsize*h.shstrndx);

    elf_sheader shstr = read_obj<elf_sheader>(*stream);
    stream->seekg(shstr.offset);

    vector<char> raw_shstrtab;
    raw_shstrtab.reserve(shstr.size);
    copy_n(istream_iterator<char>(*stream), shstr.size, raw_shstrtab.begin());

    stream->seekg(h.shoff);

    for (int i = 0; i < h.shnum; i++) {
        elf_sheader sec = read_obj<elf_sheader>(*stream);
        string name{&raw_shstrtab[sec.name]};
        sections[name] = { sec.offset, sec.size };
    }
}

elf::sectionstream elf::get_section(string name)
{
    section sec = sections.at(name);
    return sectionstream(stream, sec);
}

elf::sectionstream::sectionstream(std::shared_ptr<std::istream> stream,
                                  section sec)
    : istream(stream->rdbuf()), sec(sec), stream(stream)
{
    seekg(0);
}

elf::sectionstream::pos_type elf::sectionstream::tellg()
{
    return stream->tellg() - pos_type(sec.offset);
}

elf::sectionstream & elf::sectionstream::seekg(elf::sectionstream::pos_type pos)
{
    stream->seekg(pos + pos_type(sec.offset));
    return *this;
}

elf::sectionstream::int_type elf::sectionstream::get()
{
    if (tellg() > pos_type(sec.size))
        return EOF;

    return stream->get();
}

};
