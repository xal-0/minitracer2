#include "dwarf.hpp"

using namespace std;

namespace dwarf {

dwarf::dwarf(istream &stream)
    : stream(stream), elf(stream)
{}

size_t soffset;

void dwarf::read_linenums()
{
    elf::elf::section sec;
    try {
        sec = elf.get_section(".debug_line");
    } catch (const out_of_range &e) {
        throw invalid_argument("no DWARF linenum info found");
    }

    stream.seekg(sec.offset);

    soffset = sec.offset;

    while (stream.tellg() < static_cast<long>(sec.size + sec.offset)) 
        linenum_prog p {*this};

    for (const auto &x : line_mappings)
        cout << hex << x.address << "\t\t\t" << x.file->filename << ":" << dec << x.line << "\n";
}

bool operator<(const dwarf::meta_file &a, const dwarf::meta_file &b)
{
    return a.filename < b.filename;
}

dwarf::linenum_prog::linenum_prog(dwarf &d)
{
    u32 start = d.stream.tellg();
    header = read_obj<linenum_header>(d.stream);

    if (header.unit_length == 0xffffffff)
        throw invalid_argument("only 32-bit DWARF is supported");

    if (header.version > 2)
        throw invalid_argument("only DWARF2 is supported");

    standard_opcode_lengths.reserve(header.opcode_base - 1);
    for (int i = 0; i < header.opcode_base - 1; i++)
        standard_opcode_lengths.push_back(read_obj<u8>(d.stream));

    string dir;
    while (!(dir = read_str(d.stream)).empty()) {
        const string *ref = &*d.include_directories.insert(dir).first;
        include_directories.push_back(ref);
    }

    string filename;
    while (!(filename = read_str(d.stream)).empty()) {
        meta_file file;
        file.filename = filename;
        file.directory = include_directories[read_leb(d.stream)];
        file.last_modified = read_leb(d.stream);
        file.len = read_leb(d.stream);
        const meta_file *ref = &*d.file_names.insert(file).first;
        file_names.push_back(ref);
    }

    is_stmt = header.default_is_stmt;

    while (d.stream.tellg() < header.unit_length + start + 4) 
        execute(d);
}

void dwarf::linenum_prog::execute(dwarf &d)
{
    u8 op = read_obj<u8>(d.stream);

    if (op == 0)
        execute_extended(d);
    else if (op < header.opcode_base)
        execute_standard(d, op);
    else {
        line += header.line_base +
            ((op - header.opcode_base) % header.line_range);
        address += ((op - header.opcode_base) / header.line_range)
            * header.minimum_instruction_length;
        basic_block = false;

        copy_matrix(d);
    }
}

void dwarf::linenum_prog::execute_extended(dwarf &d)
{
    u32 len = read_leb(d.stream);
    u8 op = read_obj<u8>(d.stream);

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

    default:
        for (int i = 0; i < standard_opcode_lengths[op - 1]; i++)
            read_leb(d.stream);
    }
}

bool operator<(dwarf::line_map const &a, dwarf::line_map const &b)
{
    return a.address < b.address;
}

void dwarf::linenum_prog::copy_matrix(dwarf &d)
{
    line_map m;
    m.address = address;
    m.line = line;
    m.file = file_names[file - 1];

    d.line_mappings.insert(m);
}

};
