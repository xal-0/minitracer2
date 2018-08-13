#pragma once

#include <cstdint>
#include <istream>
#include <string>

namespace minitracer {

using u8 = uint8_t;
using u16 = uint16_t;
using u32 = uint32_t;
using u64 = uint64_t;

using i8 = int8_t;
using i16 = int16_t;
using i32 = int32_t;
using i64 = int64_t;

#if defined X86
using uaddr = uint32_t;
#elif defined AMD64
using uaddr = uint64_t;
#else
#error "unsupported platform"
#endif

template <typename T>
T read_obj(std::istream &s)
{
    T x {};
    s.read(reinterpret_cast<char*>(&x), sizeof(T));
    return x;
}

u32 read_leb(std::istream &s);
i32 read_sleb(std::istream &s);
std::string read_str(std::istream &s);

}
