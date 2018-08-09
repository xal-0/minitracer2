#include <istream>
#include <cstdint>

template <typename T>
T read_obj(std::istream &s)
{
    T x;
    s.read(reinterpret_cast<char*>(&x), sizeof(T));
    return x;
}

uint32_t read_leb(std::istream &s);
