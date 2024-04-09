#include "graphics/camera.hpp"

Camera::Camera(glm::vec3 pos, glm::vec3 dir, glm::vec3 up): pos(pos), dir(dir), up(up){
	update_mat_vp();
}

void Camera::update_mat_vp(){
	mat_vp = glm::perspective(glm::radians(70.f), 320.f/240.f, 0.001f, 1000.f)
           * glm::lookAt(pos, pos+dir, up);
}
