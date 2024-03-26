#include "utils/adc.hpp"

// -- Globals

static volatile adc_reg_t* adc_reg_ptr_ = reinterpret_cast<adc_reg_t*>(ADC_BASE);

// -- Public

void adc_set_auto_update(bool auto_update) {
    adc_reg_ptr_->channel_1 = auto_update ? 1 : 0;
}

void adc_req_update() {
    adc_reg_ptr_->channel_0 = 1;
}

// Returns -1 (0xFFFF) if channel invalid
u16 adc_read_channel(u8 channel) {
    if (channel >= 0 && channel <= 7) {
        u16 raw = *(reinterpret_cast<volatile u32*>(adc_reg_ptr_) + channel);   // slightly scuffed
        // u16 raw = adc_reg_ptr_->channel_0;
        return raw & 0x0FFF;
    }
    else return -1;
}
