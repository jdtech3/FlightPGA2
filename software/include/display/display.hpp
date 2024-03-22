#pragma once

#include <vector>
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
        std::vector<std::unique_ptr<DisplayObject>> display_objs_;

        void draw_current_frame_();
        void erase_last_frame_();
        void swap_buffer_blocking_();

    public:
        u32 cur_frame;

        Display(u32 pixel_buf_controller_addr);
        void clear(u8 buf = 0);
        void draw_frame();
        void add_display_obj(std::unique_ptr<DisplayObject> display_obj);
        
        template <class T>
        void add_display_obj(T&& display_obj) {
            display_obj.frame_id = cur_frame;
            display_objs_.push_back(std::make_unique<T>(display_obj));
        }
};