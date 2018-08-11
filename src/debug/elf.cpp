#include <vector>
#include <iterator>
#include <algorithm>

#include "elf.hpp"
#include "byteio.hpp"
#include "arch/arch.hpp"

using namespace std;

namespace minitracer {

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
