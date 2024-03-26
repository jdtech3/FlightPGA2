#include "utils/utils.hpp"

// Init random
void init_random() {
    u32 seed = 0;
    std::cout << "RNG init: seeding from ADC samples: ";
    for (u8 i = 0; i < 4; i++) {        // read the 8 channels 4 times for 32 bits
        adc_req_update();
        for (u8 j = 0; j < 8; j++) {
            seed = (seed << 1) + (adc_read_channel(j) & 1);
            std::cout << adc_read_channel(j) << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "RNG init: final seed: " << seed << std::endl;
    std::srand(seed);
}

// Random int from low inclusive to high not inclusive
int randint(int low, int high) {
    return (std::rand() % (high - low)) + low;
}
