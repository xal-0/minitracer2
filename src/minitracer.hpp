#pragma once

#include "arch/arch.hpp"
#include "debug/debuginfo.hpp"

#include <vector>

namespace minitracer {
namespace tracer {

struct trace_entry {
    uintptr_t addr;
};

void init(const char *exec_path);

std::vector<uintptr_t> trace();

void print();

}
}
