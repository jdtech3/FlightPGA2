#include "display/primitives.hpp"

// -- Pixel

Pixel::Pixel(u16 x, u16 y, u16 color) {
    this->x = x;
    this->y = y;
    this->color = color;
}

void Pixel::draw(u16* pixel_buf) const {
    draw(pixel_buf, x, y, color);
}

void Pixel::draw(u16* pixel_buf, u16 x, u16 y, u16 color) {     // static
    auto pixel_addr = reinterpret_cast<u16*>(reinterpret_cast<int>(pixel_buf) + (static_cast<int>(y) << 10) + (static_cast<int>(x) << 1));
    *pixel_addr = color;
}

// -- Line

Line::Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color) {
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
    this->color = color;
}

void Line::draw(u16* pixel_buf) const {
    int temp_x1 = x1;
    int temp_y1 = y1;
    int temp_x2 = x2;
    int temp_y2 = y2;
    
    bool is_steep = abs(temp_y2 - temp_y1) > abs(temp_x2 - temp_x1);
    if (is_steep) {
        swap(&temp_x1, &temp_y1);
        swap(&temp_x2, &temp_y2);
    }
    if (temp_x1 > temp_x2) {
        swap(&temp_x1, &temp_x2);
        swap(&temp_y1, &temp_y2);
    }

    int dx = temp_x2 - temp_x1;       // no need for abs here as already guaranteed by above
    int dy = abs(temp_y2 - temp_y1);
    int err = -(dx / 2);
    int y = temp_y1;
    int y_step = (temp_y1 < temp_y2) ? 1 : -1;

    for (int x = temp_x1; x <= temp_x2; x++) {
        if (is_steep) Pixel::draw(pixel_buf, y, x, color);
        else Pixel::draw(pixel_buf, x, y, color);

        err += dy;
        if (err > 0) {
            y += y_step;
            err -= dx;
        }
    }
}

// -- Rectangle

Rectangle::Rectangle(u16 x, u16 y, u16 width, u16 height, u16 color) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
    this->color = color;
}

void Rectangle::draw(u16* pixel_buf) const {
    for (int i = x; i < (x + width); i++)
        for (int j = y; j < (y + height); j++)
            Pixel::draw(pixel_buf, i, j, color);
}
