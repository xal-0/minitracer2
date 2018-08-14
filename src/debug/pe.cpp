#include "pe.hpp"

#include <algorithm>
#include <stdexcept>

using std::ios_base;
using std::array;
using std::equal;
using std::invalid_argument;

namespace minitracer {

pe::pe(std::istream &stream)
    : stream(stream)
{
    read_sections();
}

void pe::read_sections()
{
    stream.exceptions(ios_base::badbit | ios_base::failbit | ios_base::eofbit);

    array<char, 4> magic;
    stream.seekg(0x3c);
    auto pe_off = read_obj<u32>(stream);
    stream.seekg(pe_off);
    
    stream.read(magic.data(), magic.size());

    if (!equal(magic.begin(), magic.end(), pe_magic.begin()))
        throw invalid_argument {"not a pe file (wrong magic)"};
    
    auto header = read_obj<coff_header>(stream);

    if (header.machine != pe_machine)
        throw invalid_argument {"machine type mismatch"};

    read_obj<opt_header>(stream);
    auto w_header = read_obj<windows_header>(stream);
    stream.seekg(0x8 * w_header.data_directory_num, ios_base::cur);

    for (int i = 0; i < header.sections_num; i++) {
        auto sec = read_obj<section_header>(stream);
        std::cout << sec.name << "\n";
    }
}

pe::section pe::get_section(std::string name)
{
    return {};
}

}
