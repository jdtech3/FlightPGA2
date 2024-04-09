#pragma once

#include "cstdint_short.hpp"
#include "address_map_nios2.h"
#include "utils/logging.hpp"


namespace hex_display {
    typedef struct hex_display_reg {
        u32 hex0_3;
        u32 __1;
        u32 __2;
        u32 __3;
        u32 hex4_5;
    } hex_display_reg_t;

    void init();
    void display_uint(u32 num);
}