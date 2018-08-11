#pragma once

#include <iostream>
#include <vector>
#include <string>

#include "elf.hpp"
#include "arch/arch.hpp"
#include "byteio.hpp"

namespace dwarf {

class dwarf {
public:
    dwarf(std::istream &stream);
    
    void read_linenums();

protected:
    class linenum_prog {
    public:
        linenum_prog(std::istream &stream);

    protected:
        STARTPACK
        struct PACK linenum_header {
            u32 unit_length;
            u16 version;
            u32 header_length;
            u8 minimum_intstruction_length;
            u8 default_is_stmt;
            i8 line_base;
            u8 line_range;
            u8 opcode_base;
        };
        ENDPACK

    private:
        linenum_header header;
        std::vector<u8> standard_opcode_lengths;
        std::vector<std::string> include_directories;
        std::vector<std::string> file_names;
    };

private:
    std::istream &stream;
    elf::elf elf;
};

};
