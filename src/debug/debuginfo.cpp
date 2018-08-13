#include "debuginfo.hpp"

namespace minitracer {

bool operator<(const debug_info::meta_file &a, const debug_info::meta_file &b)
{
    return a.filename < b.filename;
}

bool operator<(debug_info::line_map const &a, debug_info::line_map const &b)
{
    return a.address < b.address;
}

}
