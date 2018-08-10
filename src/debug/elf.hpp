#pragma once

#include <cstdint>
#include <iostream>
#include <array>
#include <string>
#include <map>
#include <memory>
#include <streambuf>

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
        size_t offset;
        size_t size;
    };

public:
    class sectionstream : public std::istream {
    public:
        sectionstream(std::shared_ptr<std::istream>,
                      section sec);

        pos_type tellg();
        sectionstream & seekg(pos_type pos);

        int_type get();
        
        section sec;

    private:
        std::shared_ptr<std::istream> stream;
    };
    
    elf(std::string filename);
    elf(std::shared_ptr<std::istream> stream);

    sectionstream get_section(std::string name);

private:
    void read_sections();
    std::shared_ptr<std::istream> stream;
    std::map<std::string, section> sections;
};

};
