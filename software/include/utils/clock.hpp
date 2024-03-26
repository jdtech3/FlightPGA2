#pragma once

#include <sys/alt_irq.h>
#include <nios2.h>

#include "cstdint_short.hpp"


typedef struct interval_timer_reg {
    u16 status;                 u16 __1;
    u16 control;                u16 __2;
    u16 counter_start_low;      u16 __3;
    u16 counter_start_high;     u16 __4;
    u16 counter_snapshot_low;   u16 __5;
    u16 counter_snapshot_high;  u16 __6;
} interval_timer_reg_t;

void init_timer_isr();
u32 get_clock_ms();
void reset_clock_ms();
