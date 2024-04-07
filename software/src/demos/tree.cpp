#include "demos/demos.hpp"

#include "address_map_nios2.h"
#include "constants.hpp"
#include "display/display.hpp"
#include "display/primitives.hpp"
#include "graphics/shapes.hpp"
#include "graphics/mesh.hpp"
#include "game/assets.hpp"
#include "utils/utils.hpp"
#include "utils/clock.hpp"
#include "utils/logging.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtx/rotate_vector.hpp>

#include <cstring>
#include <vector>
#include <algorithm>
#include <memory>

#define TREE_BUSH_SIDE 5.f
#define TREE_BUSH_HEIGHT 30.f
#define TREE_BUSH_COLOR 0x16A1
#define TREE_TRUNK_SIDE 0.5f
#define TREE_TRUNK_HEIGHT 3.f
#define TREE_TRUNK_COLOR 0x8220
// #define TREE_TRUNK_ENABLE

const Mesh tree_mesh = {
    {0, 0, 0},
    {
        {0.f, 0.f, TREE_BUSH_HEIGHT+TREE_TRUNK_HEIGHT},
        {-TREE_BUSH_SIDE, +TREE_BUSH_SIDE, TREE_TRUNK_HEIGHT},
        {-TREE_BUSH_SIDE, -TREE_BUSH_SIDE, TREE_TRUNK_HEIGHT},
        {+TREE_BUSH_SIDE, -TREE_BUSH_SIDE, TREE_TRUNK_HEIGHT},
        {+TREE_BUSH_SIDE, +TREE_BUSH_SIDE, TREE_TRUNK_HEIGHT},
        #ifdef TREE_TRUNK_ENABLE
            {-TREE_TRUNK_SIDE, +TREE_TRUNK_SIDE, TREE_TRUNK_HEIGHT},
            {-TREE_TRUNK_SIDE, -TREE_TRUNK_SIDE, TREE_TRUNK_HEIGHT},
            {+TREE_TRUNK_SIDE, -TREE_TRUNK_SIDE, TREE_TRUNK_HEIGHT},
            {+TREE_TRUNK_SIDE, +TREE_TRUNK_SIDE, TREE_TRUNK_HEIGHT},
            {-TREE_TRUNK_SIDE, +TREE_TRUNK_SIDE, 0.f},
            {-TREE_TRUNK_SIDE, -TREE_TRUNK_SIDE, 0.f},
            {+TREE_TRUNK_SIDE, -TREE_TRUNK_SIDE, 0.f},
            {+TREE_TRUNK_SIDE, +TREE_TRUNK_SIDE, 0.f}
        #endif
    },
    {
        // Tree Bush
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        // Tree Trunk
        #ifdef TREE_TRUNK_ENABLE
            5, 6, 9,
            6, 9, 10,
            6, 7, 10,
            7, 10, 11,
            11, 7, 12,
            7, 12, 8,
            8, 12, 5,
            12, 5, 9
        #endif
    },
    {
        // Tree Bush
        glm::normalize(glm::vec3(-TREE_BUSH_HEIGHT, 0.f, TREE_BUSH_SIDE)),
        glm::normalize(glm::vec3(0.f, -TREE_BUSH_HEIGHT, TREE_BUSH_SIDE)),
        glm::normalize(glm::vec3(TREE_BUSH_HEIGHT, 0.f, TREE_BUSH_SIDE)),
        glm::normalize(glm::vec3(0.f, TREE_BUSH_HEIGHT, TREE_BUSH_SIDE)),
        // Tree Trunk
        #ifdef TREE_TRUNK_ENABLE
            {-1.f, 0.f, 0.f},
            {-1.f, 0.f, 0.f},
            {0.f, -1.f, 0.f},
            {0.f, -1.f, 0.f},
            {1.f, 0.f, 0.f},
            {1.f, 0.f, 0.f},
            {0.f, 1.f, 0.f},
            {0.f, 1.f, 0.f}
        #endif
    },
    {
        // Tree Bush
        TREE_BUSH_COLOR,
        TREE_BUSH_COLOR,
        TREE_BUSH_COLOR,
        TREE_BUSH_COLOR,
        // Tree Trunk
        #ifdef TREE_TRUNK_ENABLE
            TREE_TRUNK_COLOR,
            TREE_TRUNK_COLOR,
            TREE_TRUNK_COLOR,
            TREE_TRUNK_COLOR,
            TREE_TRUNK_COLOR,
            TREE_TRUNK_COLOR,
            TREE_TRUNK_COLOR,
            TREE_TRUNK_COLOR
        #endif
    }
};

constexpr float tree_spacing = 100;
constexpr float tree_render_distance = 500;

void draw_trees(Display& display, const glm::mat4& mat_vp, const glm::vec3 camera_dir, const glm::vec3& camera_pos, const glm::vec3& light_dir){
    
    float x_closest = std::roundf(camera_pos.x/tree_spacing)*tree_spacing;
    float y_closest = std::roundf(camera_pos.y/tree_spacing)*tree_spacing;

    for(float s = 0; s <= tree_render_distance; s+=tree_spacing){
        float x_min = x_closest-s;
        float y_min = y_closest-s;
        float x_max = x_closest+s;
        float y_max = y_closest+s;

        auto translate_and_draw = [&](float x, float y){
            glm::mat4 mat_model = glm::translate(glm::vec3(x,y,0));
            tree_mesh.add_to_frame(display, mat_model, mat_vp, camera_dir, camera_pos, light_dir);
        };

        for(float x = x_min; x <= x_max; x+=tree_spacing){
            int y = y_min;
            translate_and_draw(x,y);
            y = y_max;
            translate_and_draw(x,y);
        }

        for(float y = y_min; y <= y_max; y+=tree_spacing){
            int x = x_min;
            translate_and_draw(x,y);
            x = x_max;
            translate_and_draw(x,y);
        }

    }
}

int tree(){

    logging::info("Tree", "launching!");
    Display display(PIXEL_BUF_CTRL_BASE);

    int camera_distance = 200;

    glm::vec3 camera_pos = glm::vec3(0, -camera_distance, 50);
    glm::vec3 camera_dir = glm::vec3(0, 1, 0);
    glm::vec3 camera_up = glm::vec3(0, 0, 1);
    
    glm::vec3 light_dir = glm::normalize(glm::vec3(1, 0, -1));

    init_timer_isr();
    init_random();

    float angle = 0;

    while(true){
        angle += 0.05;

        glm::vec3 rotated_camera_dir = glm::rotate(camera_dir, angle, glm::vec3(0, 0, 1));

        auto mat_vp = glm::perspective(glm::radians(70.f), 320.f/240.f, 0.001f, 1000.f)
                    * glm::lookAt(camera_pos, camera_pos+rotated_camera_dir, camera_up);
        
        // auto mat_model = glm::rotate(angle, glm::vec3(0, 0, 1))
        //                * glm::translate(-tree_mesh.center);

        // tree_mesh.add_to_frame(display, mat_model, mat_vp, glm::vec3(0, 1, 0), glm::vec3(0, -camera_distance, 50), light_dir);

        draw_trees(display, mat_vp, rotated_camera_dir, camera_pos, light_dir);

        display.draw_frame();

        if (display.cur_frame_id % 60 == 0) {  // only update once a second
            logging::info("Stats", "FPS: " + std::to_string(get_fps(display)) + " | Frame: " + std::to_string(display.cur_frame_id));
        }
    }

    return 0;
}


