#include "game/assets.hpp"

namespace assets{
    const Mesh ground_strip_mesh = {
        {0, 0, 0},
        {
            {-constants::GND_SIZE_SIDE, 0.f, 0.f},
            {-constants::GND_SIZE_SIDE, +constants::GND_SIZE_FORWARD, 0.f},
            {+constants::GND_SIZE_SIDE, +constants::GND_SIZE_FORWARD, 0.f},
            {+constants::GND_SIZE_SIDE, 0.f, 0.f}
        },
        {
            0, 1, 3,
            1, 3, 2
        },
        {
            {0.f, 0.f, 1.f},
            {0.f, 0.f, 1.f}
        },
        {
            0x8260, 0x8260
        }
    };

}