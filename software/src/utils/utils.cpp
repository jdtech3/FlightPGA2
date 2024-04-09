#include "utils/utils.hpp"

// Init random
void init_random() {
    u32 seed = 0;
    logging::info("RNG init", "seeding from ADC samples: ", false);
    for (u8 i = 0; i < 4; i++) {        // read the 8 channels 4 times for 32 bits
        adc_req_update();
        for (u8 j = 0; j < 8; j++) {
            seed = (seed << 1) + (adc_read_channel(j) & 1);
            std::cout << adc_read_channel(j) << ", ";
        }
    }
    std::cout << std::endl;

    logging::ok("RNG init", "done, final seed: " + std::to_string(seed));
    std::srand(seed);
}

// Random int from low inclusive to high not inclusive
int randint(int low, int high) {
    return (std::rand() % (high - low)) + low;
}

// RESET key
static void reset() {
    logging::warn("FlightGPA2", "resetting!");
    u32 start = get_clock_ms();
    while ((get_clock_ms() - start) < 1000);

    NIOS2_WRITE_STATUS(0);
    NIOS2_WRITE_IENABLE(0);
    ((void (*) (void)) NIOS2_RESET_ADDR) ();
}
void check_reset() {
    volatile u8* key_ptr = reinterpret_cast<volatile u8*>(KEY_BASE + 0xC);
    u8 keys = *key_ptr;
    *key_ptr = keys;
    
    if (keys & 1) reset();
}

// FPS
float get_fps(Display& display) {
    static u32 prev_time;
    static u32 prev_frame_id;

    u32 cur_time = get_clock_ms();
    float fps = static_cast<float>(display.cur_frame_id - prev_frame_id) / (static_cast<float>(cur_time - prev_time) / 1000.0f);

    prev_time = cur_time;
    prev_frame_id = display.cur_frame_id;

    return fps;
}
