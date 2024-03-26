#pragma once

#include <iostream>
#include <array>
#include <algorithm>
#include <sys/alt_irq.h>

#include "cstdint_short.hpp"

#define PS2_TIMEOUT 100000


typedef struct ps2_reg {
    u32 data;
    u32 control;
} ps2_reg_t;

typedef struct mouse_pos {
    float x;        // both in mm
    float y;
} mouse_pos_t;

typedef struct read_result {
    u8 data;
    bool timed_out;
} read_result_t;

void init_mouse_isr();
float get_mouse_mm_x();
float get_mouse_mm_y();
