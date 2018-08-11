#pragma once

#define STARTPACK
#define ENDPACK
#define PACK __attribute__((packed))

#ifdef AMD64
#include "amd64.hpp"
#else
#error "unsupported platform"
#endif
