#include <algorithm>

#include "dwarf.hpp"

using std::istream;
using std::ios_base;
using std::invalid_argument;
using std::out_of_range;
using std::string;

namespace minitracer {

dwarf::dwarf(istream &stream, sectioned_binary &binary)
    : stream(stream), binary(binary)
{
    read_linenums();

    for (const auto &l : line_mappings) {
        std::cout << std::hex << l.address << "\t\t" << l.file->filename << ":" << std::dec << l.line << "\n";
    }
}

void dwarf::read_linenums()
{
    elf::section sec {};
    try {
        sec = binary.get_section(".debug_line");
    } catch (const out_of_range &) {
        throw invalid_argument("no DWARF linenum info found");
    }

    stream.seekg(sec.offset);

    while (stream.tellg() < sec.size + sec.offset)
        linenum_prog p {*this};
}

dwarf::line_map dwarf::get_linenum(uaddr addr)
{
    auto it = find_if(line_mappings.begin(),
                      line_mappings.end(),
                      [addr](const line_map &m){
                          return addr < m.address;
                      });

    if (it == line_mappings.begin())
        throw invalid_argument("no such address found");
    it--;

    return *it;
}

dwarf::linenum_prog::linenum_prog(dwarf &d)
{
    istream::pos_type start = d.stream.tellg();
    header = read_obj<linenum_header>(d.stream);
    
    if (header.unit_length == 0xffffffff)
        throw invalid_argument("only 32-bit DWARF is supported");

    if (header.version > 2)
        throw invalid_argument("only DWARF2 is supported");

    standard_opcode_lengths.reserve(header.opcode_base - 1);
    for (int i = 0; i < header.opcode_base - 1; i++)
        standard_opcode_lengths.push_back(read_obj<u8>(d.stream));

    string dirname;
    while (!(dirname = read_str(d.stream)).empty()) {
        const string *ref = &*d.include_directories.insert(dirname).first;
        include_directories.push_back(ref);
    }

    string filename;
    while (!(filename = read_str(d.stream)).empty()) {
        meta_file dfile;
        dfile.filename = filename;
        u32 dir = read_leb(d.stream);
        if (!dir)
            dfile.directory = nullptr;
        else
            dfile.directory = include_directories[dir - 1];
        dfile.last_modified = read_leb(d.stream);
        dfile.len = read_leb(d.stream);
        const meta_file *ref = &*d.file_names.insert(dfile).first;
        file_names.push_back(ref);
    }

    is_stmt = header.default_is_stmt;

    while (d.stream.tellg() < header.unit_length + start + 4)
        execute(d);
}

void dwarf::linenum_prog::execute(dwarf &d)
{
    auto op = read_obj<u8>(d.stream);

    if (op == 0)
        execute_extended(d);
    else if (op < header.opcode_base)
        execute_standard(d, op);
    else
        execute_special(d, op);
}

void dwarf::linenum_prog::execute_extended(dwarf &d)
{
    u32 len = read_leb(d.stream);
    auto op = read_obj<u8>(d.stream);

    meta_file dfile;
    u32 dir;
    const meta_file *ref;

    switch (op) {
    case 1:
        end_sequence = true;
        copy_matrix(d);

        address = 0;
        file = 1;
        line = 1;
        column = 0;
        is_stmt = header.default_is_stmt;
        basic_block = false;
        end_sequence = false;

        break;

    case 2:
        address = read_obj<uaddr>(d.stream);
        break;

    case 3:
        dfile.filename = read_str(d.stream);
        dir = read_leb(d.stream);
        if (!dir)
            dfile.directory = nullptr;
        else
            dfile.directory = include_directories[dir - 1];
        dfile.last_modified = read_leb(d.stream);
        dfile.len = read_leb(d.stream);

        ref = &*d.file_names.insert(dfile).first;
        file_names.push_back(ref);
        break;

    default:
        d.stream.seekg(len, ios_base::cur);
    }
}

void dwarf::linenum_prog::execute_standard(dwarf &d, u8 op)
{
    switch (op) {
    case 1:
        copy_matrix(d);
        basic_block = false;
        break;

    case 2:
        address += read_leb(d.stream) * header.minimum_instruction_length;
        break;

    case 3:
        line += read_sleb(d.stream);
        break;

    case 4:
        file = read_leb(d.stream);
        break;

    case 5:
        column = read_leb(d.stream);
        break;

    case 6:
        is_stmt = !is_stmt;
        break;

    case 7:
        basic_block = true;
        break;

    case 8:
        address += ((255 - header.opcode_base) / header.line_range)
            * header.minimum_instruction_length;
        break;

    case 9:
        address += read_obj<u16>(d.stream);
        break;

    default:
        for (int i = 0; i < standard_opcode_lengths[op - 1]; i++)
            read_leb(d.stream);
        break;
    }
}

void dwarf::linenum_prog::execute_special(dwarf &d, u8 op)
{
    line += header.line_base +
        ((op - header.opcode_base) % header.line_range);
    address += ((op - header.opcode_base) / header.line_range)
        * header.minimum_instruction_length;
    basic_block = false;

    copy_matrix(d);
}

void dwarf::linenum_prog::copy_matrix(dwarf &d)
{
    line_map m {
        address,
        file_names[file - 1],
        line,
    };

    d.line_mappings.insert(m);
}

}
