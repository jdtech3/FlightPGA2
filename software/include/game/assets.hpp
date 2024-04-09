#pragma once

#include <array>

#include "cstdint_short.hpp"
#include "graphics/mesh.hpp"

namespace assets {
    extern const u16 airspeed_indicator[70][70];
    
    extern const Mesh ground_strip_mesh;
    extern const Mesh airplane_mesh;
    extern const Mesh tree_mesh;

    extern const int engine_sound[90646];
    extern const u32 engine_sound_length;
    extern const int seatbelt_sound[19644];
    extern const u32 seatbelt_sound_length;
}
