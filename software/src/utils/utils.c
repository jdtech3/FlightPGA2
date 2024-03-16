#include "utils/utils.h"

// Swap via pointers
void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Random int from low inclusive to high not inclusive
int randint(int low, int high) {
    return (rand() % (high - low)) + low;
}
