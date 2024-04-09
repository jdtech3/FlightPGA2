#pragma once

#include <cstdlib>
#include <iostream>
#include <string>

#include <system.h>
#include <nios2.h>

#include "cstdint_short.hpp"
#include "display/display.hpp"
#include "utils/adc.hpp"
#include "utils/logging.hpp"
#include "utils/clock.hpp"

void init_random();
int randint(int, int);
void check_reset();
float get_fps(Display& display);
