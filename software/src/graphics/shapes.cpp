
#include "graphics/shapes.hpp"


Triangle3D::Triangle3D(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3, u16 color):
    v1(v1), v2(v2), v3(v3), color(color){}

void Triangle3D::draw(Display& display) const{
    
    display.add_display_obj(Triangle(v1, v2, v3, color));
}

float Triangle3D::depth() const {
    return v1.z + v2.z + v3.z;
}

bool Triangle3D::sort_depth_(const std::unique_ptr<Triangle3D>& a, const std::unique_ptr<Triangle3D>& b){
    return a->depth() < b->depth();
}
       
Quad3D::Quad3D(glm::vec4 v1, glm::vec4 v2, glm::vec4 v3, glm::vec4 v4, u16 color):
    v1(v1), v2(v2), v3(v3), v4(v4), color(color){}

void Quad3D::draw(Display& display) const{
    display.add_display_obj(Triangle(v1, v2, v3, color));
    display.add_display_obj(Triangle(v2, v3, v4, color));
}

float Quad3D::depth() const {
    return v1.z + v2.z + v3.z + v4.z;
}

// bool Quad3D::sort_depth_(const std::unique_ptr<Quad3D>& a, const std::unique_ptr<Quad3D>& b){
//     return a->depth() < b->depth();
// }

bool Quad3D::sort_depth_(const Quad3D& a, const Quad3D& b){
    return a.depth() < b.depth();
}