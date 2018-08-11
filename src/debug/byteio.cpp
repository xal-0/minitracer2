#include "byteio.hpp"

uint32_t read_leb(std::istream &s)
{
    u32 ret = 0;
    int shift = 0;

    u32 b;
    do {
        b = s.get();
        ret |= (b & 0x7f) << shift;
        shift += 7;
    } while (b & 0x80);

    return ret;
}

std::string read_str(std::istream &s)
{
    std::string str;
    std::getline(s, str, '\0');
    return str;
}
