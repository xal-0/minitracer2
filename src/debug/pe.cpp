#include "pe.hpp"

#include <algorithm>
#include <iterator>
#include <stdexcept>
#include <string>
#include <vector>

using std::ios_base;
using std::array;
using std::vector;
using std::equal;
using std::copy_n;
using std::invalid_argument;
using std::string;
using std::istream_iterator;

namespace minitracer {

pe::pe(std::istream &stream)
    : stream(stream)
{
    read_sections();
}

bool pe::identify(std::istream &stream)
{
    
}

void pe::read_sections()
{
    stream.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);

    // find the offset to the PE header in the DOS stub
    stream.seekg(0x3c);
    auto pe_off = read_obj<u32>(stream);

    // if it's nonsense, then it isn't a PE file
    try {
        stream.seekg(pe_off);
        // verify the magic number (this also checks endianness)
        array<char, 4> magic;
        copy_n(istream_iterator<char>(stream), 4, magic.begin());
        if (!equal(magic.begin(), magic.end(), pe_magic.begin()))
            throw invalid_argument {"not a pe file (wrong magic)"};
    } catch (const ios_base::failure &) {
        throw invalid_argument {"not a pe file (no DOS stub)"};
    }

    // verify we have the right architecture
    auto header = read_obj<coff_header>(stream);
    if (header.machine != pe_machine)
        throw invalid_argument {"machine type mismatch"};

    // skip the useless header
    read_obj<opt_header>(stream);
    // find out how many data directory entries we have and skip them
    auto w_header = read_obj<windows_header>(stream);
    stream.seekg(0x8 * w_header.data_directory_num, ios_base::cur);

    // read in the sections
    vector<section_header> sec_headers;
    sec_headers.reserve(header.sections_num);
    for (int i = 0; i < header.sections_num; i++) {
        sec_headers.push_back(read_obj<section_header>(stream));
    }

    // find the string table and read it in (if it exists)
    vector<char> strtab;
    if (header.symtable_off) {
        stream.seekg(header.symtable_off + header.symtable_num*18);
        u32 strtab_size = read_obj<u32>(stream);
        strtab.resize(strtab_size - 4);
        copy_n(istream_iterator<char>(stream), strtab_size - 4, strtab.begin());
    }

    decode_sections(strtab, sec_headers);
}

void pe::decode_sections(std::vector<char> &strtab,
                         std::vector<section_header> &sectab)
{
    for (const auto &sec : sectab) {
        // normal names can be inserted directly
        if (sec.name[0] != '/')
            sections[sec.name] = {sec.data_off, sec.virt_size};
        else {
            // names starting with a slash need to be indexed in the strtab
            int off = atoi(&sec.name[1]);
            string name = &strtab[off - 4];
            sections[name] = {sec.data_off, sec.virt_size};
        }
    }
}

pe::section pe::get_section(std::string name)
{
    return sections.at(name);
}

}
