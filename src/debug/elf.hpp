#pragma once

#include <map>
#include <iostream>

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
    void read_sections();

private:
    std::istream &stream;
    std::map<std::string, section> sections;
};

};
