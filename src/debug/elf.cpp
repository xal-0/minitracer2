#include <algorithm>
#include <iterator>
#include <vector>

#include "elf.hpp"
#include "../arch/arch.hpp"
#include "byteio.hpp"

using std::istream, std::ios_base, std::streamoff, std::istream_iterator;
using std::equal, std::begin, std::end;
using std::invalid_argument;
using std::string, std::to_string;
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

    if (!equal(begin(h.magic), end(h.magic), elf_magic.begin()))
        throw invalid_argument {"not an elf file (wrong magic)"};

    if ((h.fileclass == 1 && elf_bits == 64)
        || (h.fileclass == 2 && elf_bits == 32))
        throw invalid_argument{"only " 
                + to_string(elf_bits)
                + " bits is supported"};

    if (h.data != 1)
        throw invalid_argument{"only native byte ordering is supported\n"};

    stream.seekg(static_cast<streamoff>(h.shoff + h.shentsize*h.shstrndx));

    auto shstr = read_obj<elf_sheader>(stream);
    stream.seekg(static_cast<streamoff>(shstr.offset));

    vector<char> raw_shstrtab;
    raw_shstrtab.reserve(shstr.size);
    copy_n(istream_iterator<char>(stream), shstr.size, raw_shstrtab.begin());

    stream.seekg(static_cast<streamoff>(h.shoff));

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
