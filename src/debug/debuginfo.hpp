#pragma once

#include "byteio.hpp"

namespace minitracer {

class debug_info {
public:
    struct meta_file {
        std::string filename;
        const std::string *directory;
        u32 last_modified;
        u32 len;

        friend bool operator<(meta_file const &a, meta_file const &b);
    };

    struct line_map {
        uaddr address;
        const meta_file *file;
        u32 line;

        friend bool operator<(line_map const &a, line_map const &b);
    };

    virtual const line_map *get_linenum(uaddr addr) = 0;

    virtual ~debug_info() = default;
};

}
