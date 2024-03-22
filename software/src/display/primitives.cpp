#include "display/primitives.hpp"

// -- DisplayObject

DisplayObject::DisplayObject(u16 color): color(color), frame_id(-1) {}

// -- Pixel

Pixel::Pixel(u16 x, u16 y, u16 color) :
    DisplayObject(color), x(x), y(y) {}

void Pixel::draw(u16* pixel_buf) const {
    draw(pixel_buf, x, y, color);
}

void Pixel::draw(u16* pixel_buf, u16 x, u16 y, u16 color) {     // static
    auto pixel_addr = reinterpret_cast<u16*>(reinterpret_cast<int>(pixel_buf) + (static_cast<int>(y) << 10) + (static_cast<int>(x) << 1));
    *pixel_addr = color;
}

// -- Line

Line::Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color) :
    DisplayObject(color), x1(x1), y1(y1), x2(x2), y2(y2) {}

void Line::draw(u16* pixel_buf) const {
    int temp_x1 = x1;
    int temp_y1 = y1;
    int temp_x2 = x2;
    int temp_y2 = y2;

    bool is_steep = abs(temp_y2 - temp_y1) > abs(temp_x2 - temp_x1);
    if (is_steep) {
        std::swap(temp_x1, temp_y1);
        std::swap(temp_x2, temp_y2);
    }
    if (temp_x1 > temp_x2) {
        std::swap(temp_x1, temp_x2);
        std::swap(temp_y1, temp_y2);
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

Rectangle::Rectangle(u16 x, u16 y, u16 width, u16 height, u16 color) :
    DisplayObject(color), x(x), y(y), width(width), height(height) {}

void Rectangle::draw(u16* pixel_buf) const {
    for (int i = x; i < (x + width); i++)
        for (int j = y; j < (y + height); j++)
            Pixel::draw(pixel_buf, i, j, color);
}

// -- Triangle

Triangle::Triangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 color) :
    DisplayObject(color), x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3) {}

void Triangle::draw(u16* pixel_buf) const{
    // u16 xmin = std::min(x1, std::min(x2, x3));
    // u16 ymin = std::min(y1, std::min(y2, y3));
    // u16 xmax = std::max(x1, std::max(x2, x3));
    // u16 ymax = std::max(y1, std::max(y2, y3));

    // u16 dx = xmax - xmin;
    // u16 dy = ymax - ymin;

    // cross123 = ;
    // cross213 = ;
    // cross312 = ;

    // for(u16 x = xmin; x <= xmax; x++){
    //     for(u16 y = ymin; y <= ymax; y++){
            
    //     }
    // }

    return;
}
