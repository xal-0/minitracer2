#pragma once

#include <cstdint>
#include <iostream>
#include <array>
#include <string>
#include <map>
#include <memory>
#include <streambuf>

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

class elf {
    class section {
    public:
        uintptr_t offset;
        uintptr_t size;
    };

public:
    class sectionbuf : public std::streambuf {
    public:
        sectionbuf(std::shared_ptr<std::istream> stream,
                   elf::section sec);

    protected:
        int underflow();

    private:
        std::shared_ptr<std::istream> stream;
        elf::section sec;
        char data;
        char buf[1024];
        unsigned long pos = 0;
    };
    
    elf(std::string filename);
    elf(std::shared_ptr<std::istream> stream);

    sectionbuf get_section(std::string name);

private:
    void read_sections();
    std::shared_ptr<std::istream> stream;
    std::map<std::string, section> sections;
};

};
