#pragma once

#include <algorithm>
#include <ostream>
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"

#include "constants.hpp"
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
        Pixel(i16 x, i16 y, u16 color);
        void draw(u16* pixel_buf) const;

        // Static helper method for other classes to use to reduce overhead
        static void draw(u16* pixel_buf, i16 x, i16 y, u16 color);

    private:
        i16 x;
        i16 y;
};

class Line : public DisplayObject {
    public:
        Line(i16 x1, i16 y1, i16 x2, i16 y2, u16 color);
        void draw(u16* pixel_buf) const;

    private:
        i16 x1, y1;
        i16 x2, y2;
};

class Rectangle : public DisplayObject {
    public:
        Rectangle(i16 x, i16 y, i16 width, i16 height, u16 color);
        void draw(u16* pixel_buf) const;

    private:
        i16 x, y;
        i16 width, height;
};

class Triangle : public DisplayObject{
    public:
        Triangle(i16 x1, i16 y1, i16 x2, i16 y2, i16 x3, i16 y3, u16 color);
        // Triangle(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, u16 color);
        Triangle(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, u16 color);
        void draw(u16* pixel_buf) const;
        friend std::ostream& operator<<(std::ostream& os, const Triangle& tri);

        // static Triangle from3D(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, u16 color);
        // static Triangle from3D(const glm::vec4& v1, const glm::vec4& v2, const glm::vec4& v3, u16 color);

    private:
        i16 x1, y1;
        i16 x2, y2;
        i16 x3, y3;
        //! the last two points share the same y-coordinate
        static void draw_flat_bottom(u16* pixel_buf, u16 color, i16 x1, i16 y1, i16 x2, i16 y2, i16 x3, i16 y3);
        //! the first two points share the same y-coordinate
        static void draw_flat_top(u16* pixel_buf, u16 color, i16 x1, i16 y1, i16 x2, i16 y2, i16 x3, i16 y3);
};

class Image : public DisplayObject {
    public:
        Image(const u16* image, i16 x, i16 y, i16 width, i16 height);     // x, y coords are top left corner
        void draw(u16* pixel_buf) const;

    private:
        const u16* image;
        i16 x, y;
        i16 width, height;
};
