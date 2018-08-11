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

    linenum_prog p {stream};
}

dwarf::linenum_prog::linenum_prog(istream &stream)
{
    header = read_obj<linenum_header>(stream);

    if (header.unit_length == 0xffffffff)
        throw invalid_argument("only 32-bit DWARF is supported");

    if (header.version != 2)
        throw invalid_argument("only DWARF2 is supported");

    standard_opcode_lengths.reserve(header.opcode_base - 1);
    for (int i = 0; i < header.opcode_base - 1; i++)
        standard_opcode_lengths[i] = read_obj<u8>(stream);

    
}

};
