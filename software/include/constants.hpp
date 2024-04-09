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
    constexpr float AMBIENT_LIGHT   = 0.5f;

    // World
    constexpr float GND_SIZE_SIDE = 1000.f;
    constexpr float GND_SIZE_FORWARD = 10.f;
    constexpr float TREE_SPACING = 600.f;
    constexpr float TREE_RENDER_DIST = 1500.f;
    constexpr float TREE_BUSH_SIDE = 12.f;
    constexpr float TREE_BUSH_HEIGHT = 50.f;
    constexpr u16 TREE_BUSH_COLOR = 0x16A1;
    constexpr float TREE_TRUNK_SIDE = 0.5f;
    constexpr float TREE_TRUNK_HEIGHT = 3.f;
    constexpr u16 TREE_TRUNK_COLOR = 0x8220;
    // note: some constants also defined in game/game.hpp
};
