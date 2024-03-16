#include "display/primitives.h"

// Draw pixel
void draw_pixel(short* pixel_buf, pixel_coord_t coord, short color) {
    short* one_pixel_address = (short*)((int) pixel_buf + (coord.y << 10) + (coord.x << 1));
    *one_pixel_address = color;
}

// Draw line
// Implementation based on pseudo-code given in lab 7 handout
void draw_line(short* pixel_buf, line_coord_t coord, short color) {
    int x1 = coord.x1;
    int y1 = coord.y1;
    int x2 = coord.x2;
    int y2 = coord.y2;
    
    bool is_steep = abs(y2 - y1) > abs(x2 - x1);
    if (is_steep) {
        swap(&x1, &y1);
        swap(&x2, &y2);
    }
    if (x1 > x2) {
        swap(&x1, &x2);
        swap(&y1, &y2);
    }

    int dx = x2 - x1;       // no need for abs here as already guaranteed by above
    int dy = abs(y2 - y1);
    int err = -(dx / 2);
    int y = y1;
    int y_step = (y1 < y2) ? 1 : -1;

    for (int x = x1; x <= x2; x++) {
        if (is_steep) draw_pixel(pixel_buf, (pixel_coord_t) {y, x}, color);
        else draw_pixel(pixel_buf, (pixel_coord_t) {x, y}, color);

        err += dy;
        if (err > 0) {
            y += y_step;
            err -= dx;
        }
    }
}

// Draw rectange
void draw_rect(short* pixel_buf, rect_coord_t coord, short color) {
    for (int x = coord.x; x < (coord.x + coord.width); x++)
        for (int y = coord.y; y < (coord.y + coord.height); y++)
            draw_pixel(pixel_buf, (pixel_coord_t) {x, y}, color);
}

// Clear screen
void clear_screen(short* pixel_buf) {
    for (int x = 0; x < PIXEL_BUF_WIDTH; x++)
        for (int y = 0; y < PIXEL_BUF_HEIGHT; y++)
            draw_pixel(pixel_buf, (pixel_coord_t) {x, y}, 0x0000);
}

// Swap buffer and block until sync complete
void swap_buffer_blocking() {
    volatile int* pixel_ctrl_ptr = (int*) PIXEL_CONTROLLER_ADDR;

    *pixel_ctrl_ptr = 1;                            // cause buffer swap
    while ((*(pixel_ctrl_ptr + 3) & 0b1) != 0);     // block until done
}
