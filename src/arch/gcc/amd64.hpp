#pragma once

#include <cstdint>

struct frame {
    struct frame *bp;
    uintptr_t ret;
};

#define MT_GET_IP(x) asm("leaq (%%rip), %0;"    \
                         : "=r"(x));

#define MT_GET_FRAME(x) asm("movq %%rbp, %0;"   \
                            : "=r"(x));

#define MT_GET_STACK(x) asm("movq %%rsp, %0;"   \
                            : "=r"(x));
#define MT_CHECK_FPTR(x) ((x) != main_bp)

#define MT_INIT                                 \
    do {                                        \
        struct frame *cframe;                   \
        MT_GET_FRAME(cframe);                   \
        top_frame = cframe->bp;                 \
    } while (0)

#define MT_NEXT_FRAME(x)                        \
    do {                                        \
        (x) = (x)->bp;                          \
    } while (0)
