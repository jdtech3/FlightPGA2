#pragma once

#include <sstream>

#include "utils/uart.hpp"
#include "utils/logging.hpp"


class Joystick {
    private:
        std::stringstream input_buf_;

    public:
        float roll;
        float pitch;
        float throttle;

        Joystick();
        bool update();
};