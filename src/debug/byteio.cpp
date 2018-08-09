#include "byteio.hpp"

uint32_t read_leb(std::istream &s)
{
    uint32_t ret = 0;
    int shift = 0;

    uint32_t b;
    do {
        b = s.get();
        ret |= (b & 0x7f) << shift;
        shift += 7;
    } while (b & 0x80);

    return ret;
}
