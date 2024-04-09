#pragma once

#include <memory>
#include <sstream>

#include "cstdint_short.hpp"
#include "display/display.hpp"
#include "display/primitives.hpp"
#include "graphics/camera.hpp"
#include "game/assets.hpp"
#include "game/plane.hpp"
#include "input/uart_joystick.hpp"
#include "input/mouse.hpp"
#include "utils/char_buf.hpp"
#include "utils/clock.hpp"
#include "utils/logging.hpp"
#include "utils/utils.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/gtx/intersect.hpp>


typedef struct enabled_hardware {
    bool joystick;
    bool ps2_keyboard;
    bool ps2_mouse;
} enabled_hardware_t;

typedef struct game_options {
    bool debug_mode;
    bool profiling_mode;
    bool show_fps;
} game_options_t;

namespace constants {
    const enabled_hardware_t default_enabled_hardware = {
        .joystick = true,
        .ps2_keyboard = false,
        .ps2_mouse = false,
    };

    const game_options_t default_game_options = {
        .debug_mode = false,
        .profiling_mode = false,
        .show_fps = true,
    };
}

class Game {
    private:
        enabled_hardware_t enabled_hardware_;
        game_options_t game_options_;

        std::unique_ptr<Display> display_;
        std::unique_ptr<CharBuf> char_buf_;
        std::unique_ptr<Joystick> joystick_;
        std::unique_ptr<Plane> plane_;

        void init_char_buf_();
        void draw_ground(const Camera& camera, const glm::vec3& light_dir);
        void draw_trees(const Camera& camera, const glm::vec3& light_dir);

    public:
        Game(
            game_options_t game_options = constants::default_game_options,
            enabled_hardware_t enabled_hardware = constants::default_enabled_hardware
        );
        int run();
};
