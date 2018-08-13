#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <set>

#include "elf.hpp"
#include "../arch/arch.hpp"
#include "byteio.hpp"
#include "debuginfo.hpp"

namespace minitracer {

class dwarf : public debug_info {
public:
    dwarf(std::istream &stream, sectioned_binary &bin);
    
    line_map get_linenum(uaddr addr);

protected:
    class linenum_prog {
    public:
        linenum_prog(dwarf &d);

    protected:
        STARTPACK
        struct PACK linenum_header {
            u32 unit_length;
            u16 version;
            u32 header_length;
            u8 minimum_instruction_length;
            u8 default_is_stmt;
            i8 line_base;
            u8 line_range;
            u8 opcode_base;
        };
        ENDPACK

        void execute(dwarf &d);
        void execute_extended(dwarf &d);
        void execute_standard(dwarf &d, u8 op);
        void execute_special(dwarf &d, u8 op);

        void copy_matrix(dwarf &d);

    private:
        linenum_header header;
        std::vector<u8> standard_opcode_lengths;
        std::vector<const std::string *> include_directories;
        std::vector<const meta_file *> file_names;

        uaddr address = 0;
        u32 file = 1;
        u32 line = 1;
        u32 column = 0;
        bool is_stmt;
        bool basic_block = false;
        bool end_sequence = false;
    };

    void read_linenums();

private:
    std::istream &stream;
    sectioned_binary &binary;

    std::set<std::string> include_directories;
    std::set<meta_file> file_names;

    std::set<line_map> line_mappings;
};

}
