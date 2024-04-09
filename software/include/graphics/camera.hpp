#pragma once

#include "glm/vec3.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/transform.hpp>


class Camera{
    public:
        glm::vec3 pos;
        glm::vec3 dir;
        glm::vec3 up;
        glm::mat4 mat_vp;

        Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up);

        void update_mat_vp();

};
