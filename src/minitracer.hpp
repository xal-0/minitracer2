#pragma once

#include "arch/arch.hpp"
#include "debug/debuginfo.hpp"

#include <vector>

namespace minitracer {
namespace tracer {

struct trace_entry {
    uintptr_t addr;

    const debug_info::line_map *info;
};

void init(const char *exec_path);

std::vector<uintptr_t> trace();
std::vector<trace_entry> trace_debug();

void print();

}
}
