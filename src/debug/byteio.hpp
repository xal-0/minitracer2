#pragma once

#include <istream>
#include <cstdint>
#include <string>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

#if X86
typedef uint32_t uaddr;
#elif AMD64
typedef uint64_t uaddr;
#else
#error "unsupported platform"
#endif

template <typename T>
T read_obj(std::istream &s)
{
    T x;
    s.read(reinterpret_cast<char*>(&x), sizeof(T));
    return x;
}

u32 read_leb(std::istream &s);
std::string read_str(std::istream &s);
