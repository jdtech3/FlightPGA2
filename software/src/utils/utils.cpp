#include "utils/utils.hpp"

// Init random
void init_random() {
    u32 seed = *reinterpret_cast<u32*>(0xDABEEF);   // 14.335 MBish
    std::cout << "RNG init: seed " << seed << std::endl;
    std::srand(seed);
}

// Random int from low inclusive to high not inclusive
int randint(int low, int high) {
    return (std::rand() % (high - low)) + low;
}
