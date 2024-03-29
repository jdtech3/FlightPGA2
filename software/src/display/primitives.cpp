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
    // NOTE: move this check to display object level if "performance is getting hairy"
    if (x < 0 || x >= PIXEL_BUF_HEIGHT || y < 0 || y >= PIXEL_BUF_WIDTH) return;

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

// static void draw_triangle_flat_bottom_(
//     u16* pixel_buf,
//     u16 color,
//     int x1, int y1,
//     int x2, int y2,
//     int x3, int y3
// ){

//     int num_left = x2-x1; // -
//     int den_left = y2-y1; // +

//     int num_right = x3-x1; // +
//     int den_right = y3-y1; // +

//     int top = y1;
//     int bottom = y3; // or y2

//     for(int y = top; y <= bottom; ++y){

//         int del_y = y-top;

//         int left = x1+(del_y*num_left)/den_left;
//         int right = x1+(del_y*num_right)/den_right;

//         for(int x = left; x <= right; ++x)
//             Pixel::draw(pixel_buf,x,y,color);
//     }
    

// }

// static void draw_triangle_flat_top_(
//     u16* pixel_buf,
//     u16 color,
//     int x1, int y1,
//     int x2, int y2,
//     int x3, int y3
// ){
//     int num_left = x3-x1; // +
//     int den_left = y3-y1; // +

//     int num_right = x3-x2; // -
//     int den_right = y3-y2; // +

//     int top = y1; // or y2
//     int bottom = y3;

//     for(int y = top; y <= bottom; ++y){

//         int del_y = y-top;

//         int left = x1+(del_y*num_left)/den_left;
//         int right = x2+(del_y*num_right)/den_right;

//         for(int x = left; x <= right; ++x)
//             Pixel::draw(pixel_buf,x,y,color);
//     }
// }

Triangle::Triangle(u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3, u16 color) :
    DisplayObject(color), x1(x1), y1(y1), x2(x2), y2(y2), x3(x3), y3(y3){

    if(this->y1 > this->y2){
        std::swap(this->y1,this->y2);
        std::swap(this->x1,this->x2);
    }
    if(this->y2 > this->y3){
        std::swap(this->y2,this->y3);
        std::swap(this->x2,this->x3);
        if(this->y1 > this->y2){
            std::swap(this->y1,this->y2);
            std::swap(this->x1,this->x2);
        }
    }

}

void Triangle::draw(u16* pixel_buf) const{

    if(y2==y3) Triangle::draw_flat_bottom(pixel_buf,color,x1,y1,x2,y2,x3,y3);
    else if(y1==y2) Triangle::draw_flat_top(pixel_buf,color,x1,y1,x2,y2,x3,y3);
    else{
        int x4 = (int)x1 + ((int)(y2-y1)*(int)(x3-x1))/(int)(y3-y1);
        int y4 = y2;
        Triangle::draw_flat_bottom(pixel_buf,color,x1,y1,x2,y2,x4,y4);
        Triangle::draw_flat_top(pixel_buf,color,x2,y2,x4,y4,x3,y3);
    }

    return;
}

void Triangle::draw_flat_bottom(u16* pixel_buf, u16 color, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3){

    if(x2>x3){
        std::swap(x2,x3);
        std::swap(y2,y3);
    }

    int num_left = x2-x1; // -
    int den_left = y2-y1; // +

    int num_right = x3-x1; // +
    int den_right = y3-y1; // +

    int top = y1;
    int bottom = y3; // or y2

    if (den_left == 0 || den_right == 0) return;    // just in case we get div/0

    for(int y = top; y <= bottom; ++y){

        int del_y = y-top;

        int left = x1+(del_y*num_left)/den_left;
        int right = x1+(del_y*num_right)/den_right;

        for(int x = left; x <= right; ++x)
            Pixel::draw(pixel_buf,x,y,color);
    }
    

}

void Triangle::draw_flat_top(u16* pixel_buf, u16 color, u16 x1, u16 y1, u16 x2, u16 y2, u16 x3, u16 y3){

    if(x1>x2){
        std::swap(x1,x2);
        std::swap(y1,y2);
    }

    int num_left = x3-x1; // +
    int den_left = y3-y1; // +

    int num_right = x3-x2; // -
    int den_right = y3-y2; // +

    int top = y1; // or y2
    int bottom = y3;

    for(int y = top; y <= bottom; ++y){

        int del_y = y-top;

        int left = x1+(del_y*num_left)/den_left;
        int right = x2+(del_y*num_right)/den_right;

        for(int x = left; x <= right; ++x)
            Pixel::draw(pixel_buf,x,y,color);
    }
}

std::ostream& operator<<(std::ostream& os, const Triangle& tri) {
    os << "Triangle: [(" << tri.x1 << "," << tri.y1 << "), " << tri.x2 << "," << tri.y2 << "), " << tri.x3 << "," << tri.y3 << ")]";
    return os;
}