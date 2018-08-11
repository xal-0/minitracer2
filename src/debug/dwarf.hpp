#pragma once

#include <iostream>
#include <vector>

#include "elf.hpp"
#include "arch/arch.hpp"
#include "byteio.hpp"

namespace dwarf {

class dwarf {
public:
    dwarf(std::istream &stream);
    
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
            u8 maximum_operations_per_instructions;
            u8 default_is_stmt;
            i8 line_base;
            u8 line_range;
            u8 opcode_base;
        };
        ENDPACK

    private:
        linenum_header header;
        std::vector<u8> standard_opcode_lengths;
    };

    void read_linenums();
    void read_line_prog_cu();
    
private:
    std::istream &stream;
    elf::elf elf;
};

};
