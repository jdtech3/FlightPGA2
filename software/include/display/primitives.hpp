#pragma once

#include <algorithm>

#include "constants.h"
#include "cstdint_short.hpp"


class DisplayObject {
    public:
        u16 color;
        i32 frame_id;

        DisplayObject(u16 color);
        virtual void draw(u16* pixel_buf) const = 0;
};

class Pixel : public DisplayObject {
    public:
        Pixel(u16 x, u16 y, u16 color);
        void draw(u16* pixel_buf) const;

        // Static helper method for other classes to use to reduce overhead
        static void draw(u16* pixel_buf, u16 x, u16 y, u16 color);

    private:
        u16 x;
        u16 y;
};

class Line : public DisplayObject {
    public:
        Line(u16 x1, u16 y1, u16 x2, u16 y2, u16 color);
        void draw(u16* pixel_buf) const;

    private:
        u16 x1, y1;
        u16 x2, y2;
};

class Rectangle : public DisplayObject {
    public:
        Rectangle(u16 x, u16 y, u16 width, u16 height, u16 color);
        void draw(u16* pixel_buf) const;

    private:
        u16 x, y;
        u16 width, height;
};


class Triangle : public DisplayObject{
    public:
        Triangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 color);
        void draw(u16* pixel_buf) const;

    private:
        u16 x1, y1;
        u16 x2, y2;
        u16 x3, y3;
        //! the last two points share the same y-coordinate
        static void draw_flat_bottom(u16* pixel_buf, u16 color, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3);
        //! the first two points share the same y-coordinate
        static void draw_flat_top(u16* pixel_buf, u16 color, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3);
};
