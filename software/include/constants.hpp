#pragma once

#include "cstdint_short.hpp"


namespace constants {
    // Math/Physics
    constexpr float PI      = 3.14159265f;
    constexpr float g       = 9.80665f;
    constexpr float RHO_AIR = 1.225f;       // 15C, sea level
    
    // Units
    constexpr float M_PER_SEC_TO_KNOTS = 1.94384f;

    // Hardware
    const u32 AUDIO_SAMPLE_RATE = 8000;
    const u32 PIXEL_BUF_WIDTH   = 320;
    const u32 PIXEL_BUF_HEIGHT  = 240;

    // Settings
    const u16 ERASE_COLOR           = 0x0000;
    constexpr float AMBIENT_LIGHT   = 0.3f;
    // note: some constants also defined in game/game.hpp
};
