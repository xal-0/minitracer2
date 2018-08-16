#include "minitracer.hpp"

#include <iostream>
#include <string>

namespace minitracer {
namespace tracer {

static frame *top_frame;
static std::string exec_file;

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

void print()
{
    auto s = trace();
    
    for (const auto &x : s) {
        std::cout << std::hex << x << "\n";
    }
}

}
}
