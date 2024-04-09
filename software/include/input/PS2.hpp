#pragma once

#include <cstdint_short.hpp>
#include <sys/alt_irq.h>
#include <stdexcept>
#include <iostream>

#define PS2_TIMEOUT 10000000

class PS2{
    public:
        int write(u8 c);
        u32 read(u8& c);
        void clear_fifo();
    private:
        u32 data;
        u32 control;
};

extern PS2 *ps2_1, *ps2_2;

