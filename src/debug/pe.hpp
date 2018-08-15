#pragma once

#include <array>
#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "../arch/arch.hpp"
#include "byteio.hpp"
#include "identifiable.hpp"
#include "sectioned.hpp"

namespace minitracer {

static const std::array<u8, 4> pe_magic = {{ 'P', 'E', 0x00, 0x00 }};

class pe : public sectioned_binary, public identifiable {
public:
    pe(std::istream &stream);
    ~pe() = default;

    section get_section(std::string name) override;
    static bool identify(std::istream &stream);

protected:
#if defined X86
    static const u16 pe_machine = 0x014c;
    static const u16 pe_opt_sig = 0x10b;
#elif defined AMD64
    static const u16 pe_machine = 0x8664;
    static const u16 pe_opt_sig = 0x20b;
#else
#error "unsupported platform"
#endif

    STARTPACK
    struct PACK coff_header {
        u16 machine;
        u16 sections_num;
        u32 time;
        u32 symtable_off;
        u32 symtable_num;
        u16 optheader_size;
        u16 characteristics;
    };

    struct PACK opt_header {
        u16 magic;
        u8 linker_ver_maj;
        u8 linker_ver_min;
        u32 code_size;
        u32 init_data_size;
        i32 uninit_data_size;
        u32 entry_point;
        u32 code_base;
#if defined X86
        u32 data_base;
#endif
    };

    struct PACK windows_header {
        uaddr image_base;
        u32 section_align;
        u32 file_align;
        u16 op_ver_maj;
        u16 op_ver_min;
        u16 image_ver_maj;
        u16 image_ver_min;
        u16 subsystem_ver_maj;
        u16 subsystem_ver_min;
        u32 win32_ver;
        u32 image_size;
        u32 header_size;
        u32 checksum;
        u16 subsystem;
        u16 dll_characteristics;
        uaddr stack_reserve;
        uaddr stack_commit;
        uaddr heap_reserve;
        uaddr heap_commit;
        u32 loader_flags;
        u32 data_directory_num;
    };

    struct PACK section_header {
        char name[0x8];
        union {
            u32 phys_addr;
            u32 virt_size;
        };
        u32 virt_addr;
        u32 data_size;
        u32 data_off;
        u32 reloc_off;
        u32 linenum_off;
        u16 reloc_num;
        u16 linenum_num;
        u32 characteristics;
    };
    ENDPACK

    void read_sections();
    void decode_sections(std::vector<char> &strtab,
                         std::vector<section_header> &sectab);

private:
    std::istream &stream;
    std::map<std::string, section> sections;
};

}
