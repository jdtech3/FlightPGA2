// #define DEBUG
// #define PROFILING_MODE
#define GLM_ENABLE_EXPERIMENTAL
// #define ENABLE_PS2_MOUSE

#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <unistd.h>

#include "address_map_nios2.h"
#include "constants.h"
#include "display/display.hpp"
#include "display/primitives.hpp"
#include "graphics/camera.hpp"
#include "utils/utils.hpp"
#include "utils/clock.hpp"
#include "input/mouse.hpp"

#include "glm/gtx/string_cast.hpp"

#define NUM_BOXES   256
#define BOX_SIZE    4
#define BOX_COLOR   0xFFFF
#define LINE_COLOR  0x881F

typedef enum direction {
    UP_LEFT,
    UP_RIGHT,
    DOWN_LEFT,
    DOWN_RIGHT,
} direction_t;

class Box {
    public:
        u16 x;
        u16 y;
        direction_t direction;
        Box(u16 x, u16 y, direction_t direction) : x(x), y(y), direction(direction) {};
};

float get_fps(Display& display) {
    static u32 prev_time;
    static u32 prev_frame_id;

    u32 cur_time = get_clock_ms();
    float fps = static_cast<float>(display.cur_frame_id - prev_frame_id) / (static_cast<float>(cur_time - prev_time) / 1000.0f);

    prev_time = cur_time;
    prev_frame_id = display.cur_frame_id;

    return fps;
}

void sleep(double sec) { usleep(sec * 1000000.0); }

int main() {
	std::cout << "FlightGPA2 Launching!" << std::endl;

    Display display(PIXEL_BUF_CTRL_BASE);

    glm::mat4 cam = camera(static_cast<float>(randint(0, 1000)) / 100.0f, glm::vec2(0.0f));
    std::cout << "GLM cam matrix test: " << glm::to_string(cam) << std::endl;

    volatile char* char_buf_start = reinterpret_cast<volatile char*>(0x09000000);
    char local_char_buf[80] = {};
    memcpy(const_cast<char*>(char_buf_start), local_char_buf, sizeof(local_char_buf));  // clear char buf

    init_random();
    init_timer_isr();
    #ifdef ENABLE_PS2_MOUSE
        init_mouse_isr();
    #endif

    std::shared_ptr<Box> boxes[NUM_BOXES];
    for (int i = 0; i < NUM_BOXES; i++)
        boxes[i] = std::make_shared<Box>(
            randint(0, PIXEL_BUF_WIDTH - BOX_SIZE),
            randint(0, PIXEL_BUF_HEIGHT - BOX_SIZE),
            (direction_t) randint(UP_LEFT, DOWN_RIGHT + 1)
        );

    while (true) {
        for (std::shared_ptr<Box>& box : boxes) {
            if ((*box).x == 0)
                (*box).direction = ((*box).direction == UP_LEFT) ? UP_RIGHT : DOWN_RIGHT;
            else if ((*box).x + BOX_SIZE == PIXEL_BUF_WIDTH - 1)
                (*box).direction = ((*box).direction == UP_RIGHT) ? UP_LEFT : DOWN_LEFT;

            if ((*box).y == 0) 
                (*box).direction = ((*box).direction == UP_LEFT) ? DOWN_LEFT : DOWN_RIGHT;
            else if ((*box).y + BOX_SIZE == PIXEL_BUF_HEIGHT - 1)
                (*box).direction = ((*box).direction == DOWN_LEFT) ? UP_LEFT : UP_RIGHT;

            switch ((*box).direction) {
                case UP_LEFT:
                    (*box).x -= 1;
                    (*box).y -= 1;
                    break;
                case UP_RIGHT:
                    (*box).x += 1;
                    (*box).y -= 1;
                    break;
                case DOWN_LEFT:
                    (*box).x -= 1;
                    (*box).y += 1;
                    break;
                case DOWN_RIGHT:
                    (*box).x += 1;
                    (*box).y += 1;
                    break;
            }
        }

        for (int i = 0; i < NUM_BOXES; i++) {
            if (i != NUM_BOXES - 1)
                display.add_display_obj(Line((*boxes[i]).x, (*boxes[i]).y, (*boxes[i-1]).x, (*boxes[i-1]).y, LINE_COLOR));

            display.add_display_obj(Rectangle((*boxes[i]).x, (*boxes[i]).y, BOX_SIZE, BOX_SIZE, BOX_COLOR));
        }

        display.draw_frame();

        if (display.cur_frame_id % 60 == 0) {   // only update once a second
            float fps = get_fps(display);
            sprintf(const_cast<char*>(char_buf_start),
                    "FPS: %.2f | Frame %ld", fps, display.cur_frame_id); // write directly to save a memory access

            #ifdef DEBUG
                strncpy(local_char_buf, const_cast<char*>(char_buf_start), sizeof(local_char_buf));
                std::cout << local_char_buf << std::endl;
                std::cout << display.display_objs_.size() << " / " << display.display_objs_.max_size() << std::endl;

                #ifdef ENABLE_PS2_MOUSE
                    std::cout << "Mouse: [x] " << get_mouse_mm_x() << " [y] " << get_mouse_mm_y() << std::endl;
                #endif
            #endif
        }

        #ifdef PROFILING_MODE
            if (display.cur_frame_id == 3600) break;
        #endif
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
