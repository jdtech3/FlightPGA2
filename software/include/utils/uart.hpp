#pragma once

#include "address_map_nios2.h"
#include "cstdint_short.hpp"


typedef struct jtag_uart_reg {
    u32 data;
    u32 control;
} jtag_uart_reg_t;

namespace uart {
    bool is_host_connected();
    u8 get_char(bool echo = true);
    bool put_char(u8 ch);
    void clear_fifo();
}