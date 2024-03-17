#include <iostream>

#include <unistd.h>

#include "address_map_nios2.h"
#include "constants.h"
#include "display/display.hpp"
#include "display/primitives.hpp"
#include "utils/utils.hpp"

#define NUM_BOXES   36
#define BOX_SIZE    2
#define BOX_COLOR   0x0000
#define LINE_COLOR  0x881F

typedef enum direction {
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
} direction_t;

typedef struct box {
    int x;
    int y;
    direction_t direction;
} box_t;

void sleep(double sec) { usleep(sec * 1000000.0); }

int main() {
	std::cout << "FlightGPA2 Launching!" << std::endl;

    Display display(PIXEL_BUF_CTRL_BASE);
    
    box_t boxes[NUM_BOXES] = {};

    for (int i = 0; i < NUM_BOXES; i++) {
        boxes[i].x = randint(0, PIXEL_BUF_WIDTH);
        boxes[i].y = randint(0, PIXEL_BUF_HEIGHT);
        boxes[i].direction = (direction_t) randint(UP_LEFT, DOWN_RIGHT + 1);
    }

    while (true) {
        for (int i = 0; i < NUM_BOXES; i++) {
            if (boxes[i].x == 0) 
                boxes[i].direction = (boxes[i].direction == UP_LEFT) ? UP_RIGHT : DOWN_RIGHT;
            else if (boxes[i].x + BOX_SIZE == PIXEL_BUF_WIDTH - 1)
                boxes[i].direction = (boxes[i].direction == UP_RIGHT) ? UP_LEFT : DOWN_LEFT;

            if (boxes[i].y == 0) 
                boxes[i].direction = (boxes[i].direction == UP_LEFT) ? DOWN_LEFT : DOWN_RIGHT;
            else if (boxes[i].y + BOX_SIZE == PIXEL_BUF_HEIGHT - 1)
                boxes[i].direction = (boxes[i].direction == DOWN_LEFT) ? UP_LEFT : UP_RIGHT;

            switch (boxes[i].direction) {
                case UP_LEFT:
                    boxes[i].x -= 1;
                    boxes[i].y -= 1;
                    break;
                case UP_RIGHT:
                    boxes[i].x += 1;
                    boxes[i].y -= 1;
                    break;
                case DOWN_LEFT:
                    boxes[i].x -= 1;
                    boxes[i].y += 1;
                    break;
                case DOWN_RIGHT:
                    boxes[i].x += 1;
                    boxes[i].y += 1;
                    break;
            }
        }

        for (int i = 0; i < NUM_BOXES; i++) {
            if (i != NUM_BOXES - 1)
                display.add_display_obj(Line(boxes[i].x, boxes[i].y, boxes[i+1].x, boxes[i+1].y, LINE_COLOR));

            display.add_display_obj(Rectangle(boxes[i].x, boxes[i].y, BOX_SIZE, BOX_SIZE, BOX_COLOR));
        }

        display.draw_frame();

        if (display.cur_frame == 3600) break;
    }

    // auto p_leds = reinterpret_cast<volatile int*>(0xFF200000);

    // int leds = 1;
    // while (true) {
    //     for (int i = 0; i < 9; i++) {
    //         leds <<= 1;
    //         *p_leds = leds;
    //         sleep(0.1);
    //     }
    //     for (int i = 0; i < 9; i++) {
    //         leds >>= 1;
    //         *p_leds = leds;
    //         sleep(0.1);
    //     }
    // }

    return 0;
}
