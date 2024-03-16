#include "display/display.h"

// -- Global vars

static stack_t* _display__op_buf;
short* _display__pixel_buf_start;
static short _display__buffer1[PIXEL_BUF_HEIGHT][512];
static short _display__buffer2[PIXEL_BUF_HEIGHT][512];

// -- Functions

void display_init() {
    volatile int* pixel_ctrl_ptr = (int*) PIXEL_CONTROLLER_ADDR;
    
    // Init frame buffers
    *(pixel_ctrl_ptr + 1) = (int) &_display__buffer1;
    swap_buffer_blocking();
    clear_screen((short*) *pixel_ctrl_ptr);

    *(pixel_ctrl_ptr + 1) = (int) &_display__buffer2;
    clear_screen((short*) *(pixel_ctrl_ptr + 1));

    // Set ptr to back buffer
    _display__pixel_buf_start = (short*) *(pixel_ctrl_ptr + 1);

    // Init op buffer
    _display__op_buf = stack_init();
}

void draw(op_t op) {
    draw_no_record_to_op_buf(op);

    // Record to op buffer
    op_t* mem_op = (op_t*) (sizeof(op_t));
    *mem_op = op;
    stack_push(_display__op_buf, mem_op);
}

void draw_no_record_to_op_buf(op_t op) {
    switch (op.type) {
        case OP_PIXEL:
            draw_pixel(_display__pixel_buf_start, op.pixel, op.color);
            break;

        case OP_LINE:
            draw_line(_display__pixel_buf_start, op.line, op.color);
            break;

        case OP_RECT:
            draw_rect(_display__pixel_buf_start, op.rect, op.color);
            break;
    }
}

void new_frame() {
    volatile int* pixel_ctrl_ptr = (int*) PIXEL_CONTROLLER_ADDR;

    swap_buffer_blocking();
    _display__pixel_buf_start = (short*) *(pixel_ctrl_ptr + 1);
}

void erase_last_frame() {
    op_t* op = (op_t*) stack_pop(_display__op_buf);
    while (op != NULL) {
        op->color = 0x0000;     // blank
        draw_no_record_to_op_buf(*op);
        op = stack_pop(_display__op_buf);
    }
}

void display_deinit() {
    stack_deinit(_display__op_buf);
}
