#pragma once

#include "constants.h"
#include "cstdint_short.hpp"
#include "utils/utils.hpp"


class DisplayObject {
    public:
        u16 color;
        u32 frame_id = -1;

        virtual void draw(u16* pixel_buf) const = 0;
};

class Pixel : public DisplayObject {
    public:
        u16 x;
        u16 y;

        Pixel(u16 x, u16 y, u16 color);
        void draw(u16* pixel_buf) const;

        // Static helper method for other classes to use to reduce overhead
        static void draw(u16* pixel_buf, u16 x, u16 y, u16 color);
};

class Line : public DisplayObject {
    public:
        u16 x1, y1;
        u16 x2, y2;

        Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
        void draw(u16* pixel_buf) const;
};

class Rectangle : public DisplayObject {
    public:
        u16 x, y;
        u16 width, height;

        Rectangle(u16 x, u16 y, u16 width, u16 height, u16 color);
        void draw(u16* pixel_buf) const;
};
