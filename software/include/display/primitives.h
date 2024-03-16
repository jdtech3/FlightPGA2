#ifndef __DISPLAY_PRIMITIVES_H
#define __DISPLAY_PRIMITIVES_H

#include <stdbool.h>

#include "constants.h"
#include "utils/utils.h"

typedef struct pixel_coord {
    int x, y;
} pixel_coord_t;

typedef struct line_coord {
    int x1, y1;
    int x2, y2;
} line_coord_t;

typedef struct rect_coord {
    int x, y;
    int width, height;
} rect_coord_t;

void draw_pixel(short*, pixel_coord_t, short);
void clear_screen(short*);
void draw_line(short*, line_coord_t, short);
void draw_rect(short*, rect_coord_t, short);
void swap_buffer_blocking();

#endif  // __DISPLAY_PRIMITIVES_H
