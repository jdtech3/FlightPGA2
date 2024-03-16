#ifndef __DISPLAY_DISPLAY_H
#define __DISPLAY_DISPLAY_H

#include <stdbool.h>

#include "utils/stack.h"
#include "display/primitives.h"

typedef enum op_type {
    OP_PIXEL,
    OP_LINE,
    OP_RECT,
} op_type_t;

typedef struct op {
    op_type_t type;
    union {
        pixel_coord_t pixel;
        line_coord_t line;
        rect_coord_t rect;
    };
    short color;
} op_t;

void display_init();
void draw(op_t);
void draw_no_record_to_op_buf(op_t);
void erase_last_frame();
void new_frame();
void display_deinit();

#endif  // __DISPLAY_DISPLAY_H
