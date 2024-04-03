#pragma once

#include <vector>
#include <tuple>
#include <glm/glm.hpp>

#include "cstdint_short.hpp"
#include "display/display.hpp"

class Mesh{
    public:
        Mesh(
            glm::vec3&& center,
            std::vector<glm::vec3>&& vertices,
            std::vector<u16>&& faces,
            std::vector<glm::vec3>&& face_normals,
            std::vector<u16>&& face_colors);
        void add_to_frame(Display& display, const glm::mat4& model, const glm::mat4& vp, const glm::vec3& camera_dir, const glm::vec3& camera_pos, const glm::vec3& light_dir) const;
    // private:
        glm::vec3 center;
        std::vector<glm::vec3> vertices;
        std::vector<u16> faces;
        std::vector<glm::vec3> face_normals;
        std::vector<u16> face_colors;
        static u16 multiply_color(u16 color, float factor);
};