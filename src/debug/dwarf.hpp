#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <set>

#include "elf.hpp"
#include "arch/arch.hpp"
#include "byteio.hpp"

namespace dwarf {

class dwarf {
public:
    dwarf(std::istream &stream);
    
    void read_linenums();

    struct meta_file {
        std::string filename;
        const std::string *directory;
        u32 last_modified;
        u32 len;

        friend bool operator<(meta_file const &a, meta_file const &b);
    };

    struct line_map {
        uaddr address;
        const meta_file *file;
        u32 line;

        friend bool operator<(line_map const &a, line_map const &b);
    };

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

private:
    std::istream &stream;
    elf::elf elf;

    std::set<std::string> include_directories;
    std::set<meta_file> file_names;

    std::set<line_map> line_mappings;
};

};
