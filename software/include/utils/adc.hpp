#pragma once

#include "address_map_nios2.h"
#include "cstdint_short.hpp"


typedef struct adc_reg {
    u32 channel_0;
    u32 channel_1;
    u32 channel_2;
    u32 channel_3;
    u32 channel_4;
    u32 channel_5;
    u32 channel_6;
    u32 channel_7;
} adc_reg_t;

void adc_set_auto_update(bool auto_update);
void adc_req_update();
u16 adc_read_channel(u8 channel);
