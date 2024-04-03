#include "graphics/mesh.hpp"

#define max(a,b) a > b ? a : b
#define min(a,b) a < b ? a : b

Mesh::Mesh(
    glm::vec3&& center,
    std::vector<glm::vec3>&& vertices,
    std::vector<u16>&& faces,
    std::vector<glm::vec3>&& face_normals,
    std::vector<u16>&& face_colors):
center(center), vertices(vertices), faces(faces), face_normals(face_normals), face_colors(face_colors){}

void Mesh::add_to_frame(Display& display, const glm::mat4& model, const glm::mat4& vp, const glm::vec3& camera_dir, const glm::vec3& camera_pos, const glm::vec3& light_dir) const{

    const glm::mat4 mvp = vp*model;

    std::vector<glm::vec3> after_mvp;
    for(const glm::vec3& vertex : vertices)
        after_mvp.emplace_back(mvp*glm::vec4(vertex,1.f));
    
    std::vector<glm::vec3> world_normals;
    for(const glm::vec3& vertex : face_normals)
        world_normals.emplace_back(model*glm::vec4(vertex,0.f));

    for(int i = 0; i < world_normals.size(); i++){

        auto normal = world_normals[i];

        // back face culling
        if(glm::dot(normal, camera_dir) > 0.f) continue;

        float ambient = 0.15f;
        float spec_light = 0.01f;

        float diffuse = 0.1*glm::dot(normal,-light_dir);
        diffuse = max(diffuse, 0.f);

        glm::vec3 view_dir = glm::normalize(camera_pos-center);
        glm::vec3 refl_dir = glm::reflect(light_dir, normal);
        float specular = glm::dot(view_dir,refl_dir);
        specular = max(specular, 0.f);
        specular = glm::pow(specular, 4);
        specular *= spec_light;

        u16 face_color = Mesh::multiply_color(face_colors[i], diffuse+ambient);

        int j = i*3;
        display.add_display_obj(Triangle(after_mvp[faces[j]], after_mvp[faces[j+1]], after_mvp[faces[j+2]], face_color));
    }
}

u16 Mesh::multiply_color(u16 color, float factor){
    int b = color & 0b11111;
    int g = (color >> 5) & 0b111111;
    int r = (color >> 11) & 0b11111;

    r *= factor;
    g *= factor;
    b *= factor;

    r = min(r, 0b11111);
    g = min(g, 0b111111);
    b = min(b, 0b11111);

    return r << 11 | g << 5 | b;
}