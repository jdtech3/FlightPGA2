#pragma once

#include "cstdint_short.hpp"


namespace constants {
    // Math/Physics
    const float PI      = 3.14159265f;
    const float g       = 9.80665f;
    const float RHO_AIR = 1.225f;       // 15C, sea level

    // Hardware
    const u32 AUDIO_SAMPLE_RATE = 8000;
    const u32 PIXEL_BUF_WIDTH   = 320;
    const u32 PIXEL_BUF_HEIGHT  = 240;

    // Settings
    const u16 ERASE_COLOR       = 0x0000;
};
