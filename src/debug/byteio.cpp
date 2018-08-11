#include "byteio.hpp"

u32 read_leb(std::istream &s)
{
    u32 ret = 0;
    unsigned shift = 0;

    u32 b;
    do {
        b = s.get();
        ret |= (b & 0x7f) << shift;
        shift += 7;
    } while (b & 0x80);

    return ret;
}

i32 read_sleb(std::istream &s)
{
    i32 ret = 0;
    unsigned shift = 0;

    u32 b;
    do {
        b = s.get();
        ret |= (b & 0x7f) << shift;
        shift += 7;
    } while (b & 0x80);
    
    if (b & 0x40)
        ret |= (~0 << shift);

    return ret;
}

std::string read_str(std::istream &s)
{
    std::string str;
    std::getline(s, str, '\0');
    return str;
}
