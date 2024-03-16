#include <iostream>

#include <unistd.h>

#include "display/display.h"
#include "display/primitives.h"
#include "utils/utils.h"

void sleep(double sec) { usleep(sec * 1000000.0); }

int main() {
	std::cout << "FlightGPA2 Launching!" << std::endl;

    auto p_leds = reinterpret_cast<volatile int*>(0xFF200000);

    int leds = 1;
    while (true) {
        for (int i = 0; i < 9; i++) {
            leds <<= 1;
            *p_leds = leds;
            sleep(0.1);
        }
        for (int i = 0; i < 9; i++) {
            leds >>= 1;
            *p_leds = leds;
            sleep(0.1);
        }
    }

    return 0;

    // display_init();

    // op_t op = {
    //     .type = OP_RECT,
    //     .rect = { 100, 100, 5, 10 },
    //     .color = 0xFFFF,
    // };

    // erase_last_frame();
    // draw(op);
    // new_frame();

    // draw_rect(_display__pixel_buf_start, (rect_coord_t) { 100, 100, 10, 10 }, 0xFFFF);
    // swap_buffer_blocking();
        
//    op_t op = {
//        .type = OP_RECT,
//        .rect = { 0, 0, 20, 30 },
//        .color = 0xFFFF,
//    };
//
//    erase_last_frame();
//    draw(op);
//    new_frame();

    // alt_printf("Hello world!");

//    op = (op_t) {
//        .type = OP_RECT,
//        .rect = { 100, 100, 20, 30 },
//        .color = 0xFFFF,
//    };
//
//    erase_last_frame();
//    draw(op);
//    new_frame();
//
//    op = (op_t) {
//        .type = OP_RECT,
//        .rect = { 200, 200, 20, 30 },
//        .color = 0xFFFF,
//    };
//
//    erase_last_frame();
//    draw(op);
//    new_frame();

    //  while (true) {
    //      int x = randint(0, 310);
    //      int y = randint(0, 230);
        
    //      op_t op = {
    //          .type = OP_RECT,
    //          .rect = { x, y, 5, 10 },
    //          .color = 0xFFFF,
    //      };

    //      erase_last_frame();
    //      draw(op);
    //      new_frame();
    //  }

    // display_deinit();
}
