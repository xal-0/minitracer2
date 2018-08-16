#include "minitracer.hpp"
#include "debug/pe.hpp"
#include "debug/elf.hpp"
#include "debug/dwarf.hpp"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <memory>
#include <string>

namespace minitracer {
namespace tracer {

static frame *top_frame;
static std::string exec_file;

static std::unique_ptr<debug_info> info;

#if defined WINDOWS
static const char path_sep = '\\';
#else
static const char path_sep = '/';
#endif

void init(const char *exec_path)
{
    MT_INIT;
    exec_file = exec_path;
}

std::vector<uintptr_t> trace()
{
    std::vector<uintptr_t> stack;
    frame *fp;
    int count = 0;

    MT_GET_FRAME(fp);
    while (MT_CHECK_FPTR(fp)) {
        stack.push_back(fp->ret);
        MT_NEXT_FRAME(fp);

        if (count++ > 1000)
            return stack;
    }

    return stack;
}

std::vector<trace_entry> trace_debug()
{
    std::vector<uintptr_t> stack = trace();
    std::vector<trace_entry> ret;

    try {
        std::ifstream stream {exec_file};

#if defined PE
        pe exe {stream};
#elif defined ELF
        elf exe {stream};
#endif

#if defined DWARF
        info = std::make_unique<dwarf>(stream, exe);
#endif

        stream.close();
    } catch (const std::exception &e) {
        throw e;
    }

    for (auto addr : stack) {
        trace_entry e;
        e.addr = addr;

        try {
            e.info = info->get_linenum(addr);
        } catch (const std::exception &_) {
            e.info = nullptr;
        }

        ret.push_back(e);
    }

    return ret;
}

void print()
{
    auto stack = trace_debug();
    
    std::cout << "stack trace:\n";
    for (const auto &e : stack) {
        std::cout << "[" << std::hex << std::setw(8) << std::setfill('0')
                  << e.addr << "]\t" << std::dec;

        if (e.info)
            if (e.info->file->directory)
                std::cout << *e.info->file->directory << path_sep << e.info->file->filename
                          << ":" << e.info->line << "\n";
            else
                std::cout << e.info->file->filename << ":" << e.info->line
                          << "\n";
        else
            std::cout << "-- no debug info --\n";
        
    }
}

}
}
