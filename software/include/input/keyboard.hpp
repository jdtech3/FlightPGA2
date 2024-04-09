#pragma once

#include "input/PS2.hpp"
#include <unordered_set>

class Keyboard{
public:
    Keyboard(PS2* ps2);
    bool is_pressed(u8 key);
    void poll();
    friend int init_ps2();
private:
    PS2* ps2;
    bool break_code = false;
    std::unordered_set<u8> pressed_keys;
};

// extern Keyboard keyboard;
