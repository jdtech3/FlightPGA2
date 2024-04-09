#include "input/uart_joystick.hpp"

Joystick::Joystick() : roll(0.f), pitch(0.f), throttle(-1.f) {
    uart::clear_fifo();
    logging::ok("UART joystick init", "done");
}

bool Joystick::update() {
    u8 ch;
    do {
        ch = uart::get_char();
        if (ch != 0) {
            input_buf_ << ch;

            if (ch == ';') {
                float temp_roll, temp_pitch, temp_throttle;
                input_buf_ >> temp_roll >> temp_pitch >> temp_throttle;

                if (input_buf_.fail()) {
                    logging::err("UART joystick update", "parsing input buf failed!");
                    input_buf_.clear();     // clear errors
                }
                else {
                    roll = temp_roll;
                    pitch = temp_pitch;
                    throttle = temp_throttle;
                }

                input_buf_.str("");         // clear buf
            }
        }
    } while (ch);
}