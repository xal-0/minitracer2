#if defined GCC || defined CLANG
#include "gcc/gcc.hpp"
#else
#error "unsupported compiler"
#endif
