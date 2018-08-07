#pragma once

#include <cstdint>

namespace elf {

#if X86
const unsigned elf_bits = 32;
const unsigned elf_off = 4;
#elif AMD64
const unsigned elf_bits = 64
const unsigned elf_off = 8;
#else
#error "unsupported platform"
#endif

struct elf_header {
    uint8_t magic[4];
    uint8_t fileclass;
    uint8_t data;
    uint8_t version;
    uint8_t pad[8]
};

};
