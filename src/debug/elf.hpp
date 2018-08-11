#pragma once

#include <map>
#include <iostream>

#include "arch/arch.hpp"
#include "byteio.hpp"

namespace minitracer {

class elf {
public:
    class section {
    public:
        size_t offset;
        size_t size;
    };

    elf(std::istream &stream);

    section get_section(std::string name);

protected:
    const std::array<u8, 4> elf_magic = { 0x7f, 'E', 'L', 'F' };

#if X86
    const unsigned elf_bits = 32;
    const unsigned elf_off = 4;
#elif AMD64
    const unsigned elf_bits = 64;
    const unsigned elf_off = 8;
#else
#error "unsupported platform"
#endif

    STARTPACK
    struct PACK elf_header {
        u8 magic[4];
        u8 fileclass;
        u8 data;
        u8 version0;
        u8 pad[9];
        u16 type;
        u16 machine;
        u32 version1;
        uaddr entry;
        uaddr phoff;
        uaddr shoff;
        u32 flags;
        u16 ehsize;
        u16 phentsize;
        u16 phnum;
        u16 shentsize;
        u16 shnum;
        u16 shstrndx;
    };

    struct PACK elf_sheader {
        u32 name;
        u32 type;
        uaddr flags;
        uaddr addr;
        uaddr offset;
        uaddr size;
        u32 link;
        u32 info;
        uaddr addralign;
        uaddr entsize;
    };
    ENDPACK

    void read_sections();

private:
    std::istream &stream;
    std::map<std::string, section> sections;
};

};
