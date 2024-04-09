#include "input/PS2.hpp"

int PS2::write(u8 c){
    data = c;
    return 0;
    // return this->read() != 0xFA;
}

// u8 PS2::read(){

//     u32 rvalid, count;
//     u8 c;

//     for(u32 i = 0; i < PS2_TIMEOUT; ++i){
//         rvalid = read(c);
//         if(rvalid) return c;
//     }

//     throw std::runtime_error("PS2 read timed out!");
    
// }

u32 PS2::read(u8& c){
    volatile u32 temp_data = data;
    u32 rvalid = (temp_data >> 15) & 1;
    c = temp_data & 0xFF;
    return rvalid;
}

void PS2::clear_fifo(){
    u8 c;
    while(this->read(c)){
        // std::cout << std::hex << static_cast<int>(c) << std::endl;
        (void)c;
    }
}

PS2* ps2_1 = reinterpret_cast<PS2*>(PS2_PORT_BASE);
PS2* ps2_2 = reinterpret_cast<PS2*>(PS2_PORT_DUAL_BASE);

// volatile PS2& ps2_1 = *p_ps2_1;
// volatile PS2& ps2_2 = *p_ps2_2;

// int init_ps2(){
//     try{
//         // if(ps2_1->write(0xFF)) return 1;
//         // if(ps2_1->read() != 0xAA) return 2;
//         keyboard.ps2 = ps2_1;
//         keyboard.ps2->clear_fifo();
//     }catch(const std::runtime_error& r){
//         return 3;
//     }
//     return 0;
// }

