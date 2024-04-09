#include "utils/hex_display.hpp"

static volatile hex_display::hex_display_reg_t* hex_display_reg_ptr_ = reinterpret_cast<hex_display::hex_display_reg_t*>(HEX3_HEX0_BASE);
static u8 lut_[] = {
    0x3F, 0x06, 0x5B, 0x4F, 0x66,
    0x6D, 0x7D, 0x07, 0x7F, 0x67
};  // digits 0-9

namespace hex_display {
    void init() {
        const u8 dash = 1 << 6;
        hex_display_reg_ptr_->hex0_3 = (dash << 24) + (dash << 16) + (dash << 8) + dash;
        hex_display_reg_ptr_->hex4_5 = (dash << 8) + dash;

        logging::ok("Hex display init", "done");
    }

    void clear() {
        hex_display_reg_ptr_->hex0_3 = 0;
        hex_display_reg_ptr_->hex4_5 = 0;
    }

    // Max: 999999
    void display_uint(u32 num) {
        hex_display::clear();
        
        hex_display_reg_ptr_->hex4_5 = lut_[num / 100000] << 8;
        num %= 100000;
        hex_display_reg_ptr_->hex4_5 += lut_[num / 10000];
        num %= 10000;
        hex_display_reg_ptr_->hex0_3 = lut_[num / 1000] << 24;
        num %= 1000;
        hex_display_reg_ptr_->hex0_3 += lut_[num / 100] << 16;
        num %= 100;
        hex_display_reg_ptr_->hex0_3 += lut_[num / 10] << 8;
        num %= 10;
        hex_display_reg_ptr_->hex0_3 += lut_[num];
    }
}