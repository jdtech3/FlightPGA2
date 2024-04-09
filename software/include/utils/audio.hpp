#pragma once

#include <string>
#include <sys/alt_irq.h>
#include <nios2.h>

#include "cstdint_short.hpp"
#include "utils/logging.hpp"


typedef struct audio_reg {
    u32 control;
    u8 rarc;
    u8 ralc;
    u8 wsrc;
    u8 wslc;
    u32 left_data;
    u32 right_data;
} audio_reg_t;

typedef struct audio_state {
    const int* data_array;
    u32 data_array_length;
    bool looping;
    float volume;
} audio_state_t;

namespace audio {
    void init_isr();
    void play(const int* audio_data, u32 audio_data_length, bool loop = true);
    void set_volume(float volume);
}