#include "demos/demos.hpp"

#include "address_map_nios2.h"
#include "constants.hpp"
#include "display/display.hpp"
#include "display/primitives.hpp"
#include "graphics/shapes.hpp"
#include "graphics/mesh.hpp"
#include "utils/utils.hpp"
#include "utils/clock.hpp"
#include "utils/logging.hpp"
#include "game/plane.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#include <cstring>
#include <vector>
#include <algorithm>
#include <memory>

extern Mesh show_object_mesh;

#define GND_SIZE_SIDE       200.f
#define GND_SIZE_FORWARD    10.f

const Mesh ground_strip_mesh = {
    {0, 0, 0},
    {
        {-GND_SIZE_SIDE, -20.f, 0.f},
        {-GND_SIZE_SIDE, -20.f, +GND_SIZE_FORWARD},
        {+GND_SIZE_SIDE, -20.f, +GND_SIZE_FORWARD},
        {+GND_SIZE_SIDE, -20.f, 0.f}
    },
    {
        0, 1, 3,
        1, 3, 2
    },
    {
        {0.f, 0.f, 1.f},
        {0.f, 0.f, 1.f}
    },
    {
        0x07E0, 0x07E0
    }
};

// const Mesh ground_mesh = {
//     {0, 0, 0},
//     {
//         {-GND_SIZE_SIDE, -20.f, -GND_SIZE_FORWARD},
//         {-GND_SIZE_SIDE, -20.f, +GND_SIZE_FORWARD},
//         {+GND_SIZE_SIDE, -20.f, +GND_SIZE_FORWARD},
//         {+GND_SIZE_SIDE, -20.f, -GND_SIZE_FORWARD},

//         {-GND_SIZE_SIDE, -20.f, 0.f},
//         {+GND_SIZE_SIDE, -20.f, 0.f}
//     },
//     {
//         0, 4, 5,
//         0, 3, 5,
//         1, 4, 2,
//         4, 2, 5
//     },
//     {
//         {0.f, 0.f, 1.f},
//         {0.f, 0.f, 1.f},
//         {0.f, 0.f, 1.f},
//         {0.f, 0.f, 1.f}
//     },
//     {
//         0x07E0, 0x07E0,
//         0xFFE0, 0XFFE0
//     }
// };

int show_object(){

    logging::info("ShowObject", "launching!");
    Display display(PIXEL_BUF_CTRL_BASE);
    Plane plane;

    int camera_distance = 50;

    init_timer_isr();
    init_random();
    float spin = 0;
    float angle = 0;
    float rot_speed = 0.5;

    logging::info("ShowObject", "# Triangles: " + std::to_string(show_object_mesh.faces.size()/3));

    while(true){
        plane.update(0, 0.05, 0, 80);

        angle += 0.5;
        spin += rot_speed;

        auto mat_cam_rot = glm::rotate(glm::radians(angle), glm::vec3(1, 0, 0));
        glm::vec3 cam_location_vec = glm::vec3(0,0,camera_distance);
        glm::vec3 cam_pointing_vec = mat_cam_rot * glm::vec4(0, 0, -1, 0);
        glm::vec3 cam_up_vec = mat_cam_rot * glm::vec4(0, 1, 0, 0);

        auto mat_vp = glm::perspective(glm::radians(70.f), 320.f/240.f, 0.001f, 1000.f)
                    * glm::lookAt(cam_location_vec, cam_location_vec + cam_pointing_vec, cam_up_vec);

        auto mat_model = glm::rotate(glm::radians(angle),glm::vec3(0,1,0))
                       * glm::rotate(glm::radians(spin),glm::vec3(1,1,1))
                       * glm::translate(-show_object_mesh.center);
        
        show_object_mesh.add_to_frame(display, mat_model, mat_vp, glm::vec3(0, 0, -1), glm::vec3(0, 0, camera_distance), glm::vec3(0, -1, 0));
        
        // auto mat_model_gnd = glm::translate(cam_location_vec);
        // ground_mesh.add_to_frame(display, mat_model_gnd, mat_vp, glm::vec3(0, 0, -1), glm::vec3(0, 0, camera_distance), glm::vec3(0, -1, 0));

        for (int i = -5; i < 5; i++) {
            auto translation_mat = glm::translate(glm::vec3(0, 0, i*GND_SIZE_FORWARD)) * glm::translate(cam_location_vec);
            ground_strip_mesh.add_to_frame(display, translation_mat, mat_vp, glm::vec3(0, 0, -1), glm::vec3(0, 0, camera_distance), glm::vec3(0, -1, 0));
        }

        display.draw_frame();

        // return 0;

        if (display.cur_frame_id % 60 == 0) {  // only update once a second
            logging::info("Stats", "FPS: " + std::to_string(get_fps(display)) + " | Frame: " + std::to_string(display.cur_frame_id));
            // logging::info("Plane", static_cast<std::string>(plane));
            logging::info("Angle", std::to_string(angle));
        }
    }

    return 0;
}


