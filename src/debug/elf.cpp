#include <algorithm>
#include <iterator>
#include <vector>

#include "elf.hpp"
#include "../arch/arch.hpp"
#include "byteio.hpp"

using std::istream;
using std::ios_base;
using std::istream_iterator;
using std::equal;
using std::begin;
using std::end;
using std::invalid_argument;
using std::string;
using std::to_string;
using std::vector;

namespace minitracer {

elf::elf(istream &stream)
    : stream(stream)
{
    read_sections();
}

void elf::read_sections()
{
    stream.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);

    auto h = read_obj<elf_header>(stream);

    // make sure it's actually an ELF file
    if (!equal(begin(h.magic), end(h.magic), elf_magic.begin()))
        throw invalid_argument {"not an elf file (wrong magic)"};

    // if the file's bit-width doesn't match our current architecture, give up
    if ((h.fileclass == 1 && elf_bits == 64)
        || (h.fileclass == 2 && elf_bits == 32))
        throw invalid_argument{"only " 
                + to_string(elf_bits)
                + " bits is supported"};

    // check for little-endianness
    if (h.data != 1)
        throw invalid_argument{"only native byte ordering is supported\n"};

    // seek to the section headers
    stream.seekg(h.shoff + h.shentsize*h.shstrndx);

    // find the section header for the section header string table
    auto shstr = read_obj<elf_sheader>(stream);
    stream.seekg(shstr.offset);

    // read in the section header string table
    vector<char> raw_shstrtab;
    raw_shstrtab.resize(shstr.size);
    copy_n(istream_iterator<char>(stream), shstr.size, raw_shstrtab.begin());

    // go back and read the rest of the sections
    stream.seekg(h.shoff);

    for (int i = 0; i < h.shnum; i++) {
        auto sec = read_obj<elf_sheader>(stream);
        string name{&raw_shstrtab[sec.name]};
        sections[name] = { sec.offset, sec.size };
    }
}

elf::section elf::get_section(string name)
{
    return sections.at(name);
}

}
