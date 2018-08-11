#include "dwarf.hpp"

using namespace std;

namespace dwarf {

dwarf::dwarf(std::istream &stream)
    : stream(stream), elf(stream)
{}

void dwarf::read_linenums()
{
    elf::elf::section sec;
    try {
        sec = elf.get_section(".debug_line");
    } catch (const out_of_range &e) {
        throw invalid_argument("no DWARF linenum info found");
    }

    stream.seekg(sec.offset);
}

dwarf::linenum_prog::linenum_prog(istream &stream)
{
    header = read_obj<linenum_header>(stream);

    if (header.unit_length == 0xffffffff)
        throw invalid_argument("only DWARF32 is supported");

    standard_opcode_lengths.reserve(header.opcode_base - 1);
    for (int i = 0; i < 0; i++)
        standard_opcode_lengths[i] = read_obj<u8>(stream);

    
}

};
