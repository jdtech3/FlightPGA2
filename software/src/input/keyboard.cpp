#include "input/keyboard.hpp"

Keyboard::Keyboard(PS2* ps2): ps2(ps2){
    // ps2->clear_fifo();
}

bool Keyboard::is_pressed(u8 key){
    return pressed_keys.find(key) != pressed_keys.end();
}

void Keyboard::poll(){
    u8 c;
    while(ps2->read(c)){
        if(c == 0xAA || c == 0xFA || c == 0x00) continue;
        else if(c == 0xF0) break_code = true;
        else if(break_code){
            pressed_keys.erase(c);
            break_code = false;
        }else pressed_keys.insert(c);
    }
}
