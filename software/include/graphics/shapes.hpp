#pragma once

#include "cstdint_short.hpp"
#include "display/display.hpp"

#include <glm/vec3.hpp>
#include <glm/vec4.hpp>

#include <memory>

// maybe base class??

struct Triangle3D{
    glm::vec3 v1,v2,v3;
    u16 color;
    Triangle3D(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, u16 color);
    void draw(Display&) const;
    float depth() const;
    static bool sort_depth_(const std::unique_ptr<Triangle3D>& a, const std::unique_ptr<Triangle3D>& b);
};

struct Quad3D{
    glm::vec3 v1,v2,v3,v4;
    u16 color;
    Quad3D(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, glm::vec3 v4, u16 color);
    void draw(Display&) const;
    float depth() const;
    // static bool sort_depth_(const std::unique_ptr<Quad3D>& a, const std::unique_ptr<Quad3D>& b);
    static bool sort_depth_(const Quad3D& a, const Quad3D& b);
};
