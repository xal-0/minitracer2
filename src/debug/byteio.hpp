#include <iostream>
#include <cstdint>

template <typename T>
T read_obj(std::istream &s)
{
    T x;
    s.read(reinterpret_cast<char*>(&x), sizeof(T));
    return x;
}
