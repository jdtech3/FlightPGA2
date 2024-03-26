#pragma once

#include <deque>
#include <cstring>  // for memset
#include <memory>   // for smart pointers
#include <iostream>

#include "constants.h"
#include "cstdint_short.hpp"
#include "primitives.hpp"


typedef struct pixel_buffer_controller {
    u32 buffer;
    u32 back_buffer;
    u16 resolution_x;
    u16 resolution_y;
    u32 status;
} pixel_buf_controller_t;

class Display {
    private:
        volatile pixel_buf_controller_t* pixel_buf_controller_;
        u16* current_pixel_buf_;
        u16 buf1_[PIXEL_BUF_HEIGHT][512];   // 512 needed for padding (?)
        u16 buf2_[PIXEL_BUF_HEIGHT][512];

        void draw_current_frame_();
        void erase_last_frame_();
        void swap_buffer_blocking_();

    public:
        i32 cur_frame_id;
        std::deque<std::shared_ptr<DisplayObject>> display_objs_;

        Display(u32 pixel_buf_controller_addr);
        void clear(u8 buf = 0);
        void draw_frame();
        // void add_display_obj(std::shared_ptr<DisplayObject> display_obj);

        template <class T>
        void add_display_obj(T&& display_obj) {
            display_obj.frame_id = cur_frame_id;
            display_objs_.push_back(std::make_shared<T>(display_obj));
        }
};
