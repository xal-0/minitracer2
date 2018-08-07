#pragma once

#include <cstdint>
#include <iostream>
#include <array>
#include <string>
#include <map>

#define PACK __attribute__((packed))

namespace elf {

#if X86
const unsigned elf_bits = 32;
const unsigned elf_off = 4;
#elif AMD64
const unsigned elf_bits = 64;
const unsigned elf_off = 8;
#else
#error "unsupported platform"
#endif

const std::array<uint8_t, 4> elf_magic = { 0x7f, 'E', 'L', 'F' };

struct PACK elf_header {
    uint8_t magic[4];
    uint8_t fileclass;
    uint8_t data;
    uint8_t version0;
    uint8_t pad[9];
    uint16_t type;
    uint16_t machine;
    uint32_t version1;
    uintptr_t entry;
    uintptr_t phoff;
    uintptr_t shoff;
    uint32_t flags;
    uint16_t ehsize;
    uint16_t phentsize;
    uint16_t phnum;
    uint16_t shentsize;
    uint16_t shnum;
    uint16_t shstrndx;
};

struct PACK elf_sheader {
    uint32_t name;
    uint32_t type;
    uintptr_t flags;
    uintptr_t addr;
    uintptr_t offset;
    uintptr_t size;
    uint32_t link;
    uint32_t info;
    uintptr_t addralign;
    uintptr_t entsize;
};

class elf {
    class section {
    public:
        uintptr_t offset;
        uintptr_t size;
    };

public:
    elf(std::istream &s);

private:
    std::map<std::string, section> sections;
};

};
