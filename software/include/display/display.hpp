#pragma once

#include <deque>
#include <cstring>  // for memset
#include <memory>   // for smart pointers

#include "constants.hpp"
#include "cstdint_short.hpp"
#include "display/primitives.hpp"
#include "utils/logging.hpp"


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
        u16 buf1_[constants::PIXEL_BUF_HEIGHT][512];   // 512 needed for padding (?)
        u16 buf2_[constants::PIXEL_BUF_HEIGHT][512];

        void draw_current_frame_();
        void erase_last_frame_();
        void swap_buffer_();
        void swap_buffer_blocking_();

    public:
        i32 cur_frame_id;
        std::deque<std::unique_ptr<DisplayObject>> display_objs_;

        Display(u32 pixel_buf_controller_addr);

        inline bool swapping_buffer() { return (pixel_buf_controller_->status & 0b1) != 0; }
        void clear(u8 buf = 0);
        void draw_frame();

        template <class T>
        void add_display_obj(T&& display_obj) {
            display_obj.frame_id = cur_frame_id;
            display_objs_.push_back(std::make_unique<T>(display_obj));
        }
};
