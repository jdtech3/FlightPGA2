#include "display/display.hpp"

// -- Constructors

Display::Display(u32 pixel_buf_controller_addr) {
    // Init vars
    pixel_buf_controller_ = reinterpret_cast<pixel_buf_controller_t*>(pixel_buf_controller_addr);
    current_pixel_buf_ = nullptr;
    cur_frame_id = 0;

    // Init frame buffers
    clear(1);
    clear(2);
    pixel_buf_controller_->back_buffer = reinterpret_cast<u32>(&buf1_);
    swap_buffer_blocking_();
    pixel_buf_controller_->back_buffer = reinterpret_cast<u32>(&buf2_);
}

// -- Private methods

void Display::draw_current_frame_() {
    if (display_objs_.empty()) return;

    auto iter = display_objs_.rbegin();     // current frame objs are at the back, so reverse iterate
    while ((**iter).frame_id == cur_frame_id) {
        (**iter).draw(current_pixel_buf_);
        iter++;
    }
}

void Display::erase_last_frame_() {
    if (cur_frame_id < 2) return;     // no need to erase anything on first 2 frames

    while ((*display_objs_.front()).frame_id == cur_frame_id - 2) {
        (*display_objs_.front()).color = ERASE_COLOR;
        (*display_objs_.front()).draw(current_pixel_buf_);
        display_objs_.pop_front();
    }
}

void Display::swap_buffer_blocking_() {
    pixel_buf_controller_->buffer = 1;                      // cause buf swap
    while ((pixel_buf_controller_->status & 0b1) != 0);     // block until done
    current_pixel_buf_ = reinterpret_cast<u16*>(pixel_buf_controller_->back_buffer);
}

// -- Public methods

void Display::clear(u8 buf) {
    u16 (*buf_to_clear)[PIXEL_BUF_HEIGHT][512];

    switch (buf) {
        case 0:
            // This is fairly unsafe, but hardware should guarantee pixel buf pointer points to one of the bufs
            buf_to_clear = reinterpret_cast<u16 (*)[PIXEL_BUF_HEIGHT][512]>(current_pixel_buf_);
            break;
        case 1:
            buf_to_clear = &buf1_;
            break;
        case 2:
            buf_to_clear = &buf2_;
            break;
        default:
            return;     // nop
    }

    if (ERASE_COLOR == 0x0000 || ERASE_COLOR == 0xFFFF)             // optimization
        memset(buf_to_clear, ERASE_COLOR, sizeof(*buf_to_clear));
    else
        for (int x = 0; x < PIXEL_BUF_WIDTH; x++)
            for (int y = 0; y < PIXEL_BUF_HEIGHT; y++)
                (*buf_to_clear)[x][y] = ERASE_COLOR;
}

void Display::draw_frame() {
    erase_last_frame_();
    draw_current_frame_();
    swap_buffer_blocking_();
    cur_frame_id++;

    // if ((cur_frame_id % 60) == 0) {
    //     u32 lines = 0;
    //     u32 rects = 0;
    //     for (const std::unique_ptr<DisplayObject> &i : display_objs_) {
    //         if (dynamic_cast<Line*>(i.get()) != nullptr) lines++;
    //         else if (dynamic_cast<Rectangle*>(i.get()) != nullptr) rects++;
    //     }

    //     std::cout << "Current frame: " << cur_frame_id << "\n"
    //               << "Objs in buffer: " << display_objs_.size() << "\n"
    //               << "Lines in buffer: " << lines << "\n"
    //               << "Rectangles in buffer: " << rects << "\n" << std::endl;
    // }

}
