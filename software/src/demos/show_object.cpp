// #include "demos/demos.hpp"

// #include "address_map_nios2.h"
// #include "constants.hpp"
// #include "display/display.hpp"
// #include "display/primitives.hpp"
// #include "graphics/shapes.hpp"
// #include "graphics/mesh.hpp"
// #include "utils/utils.hpp"
// #include "utils/clock.hpp"
// #include "utils/logging.hpp"
// #include "game/plane.hpp"

// #define GLM_ENABLE_EXPERIMENTAL
// #include <glm/gtx/string_cast.hpp>
// #include <glm/gtx/transform.hpp>

// #include <cstring>
// #include <vector>
// #include <algorithm>
// #include <memory>

// extern Mesh show_object_mesh;

// int show_object(){

//     logging::info("ShowObject", "launching!");
//     Display display(PIXEL_BUF_CTRL_BASE);

//     int camera_distance = 25;

//     init_timer_isr();
//     init_random();
//     float spin = 0;
//     float angle = 0;
//     float rot_speed = 0.5;

//     logging::info("ShowObject", "# Triangles: " + std::to_string(show_object_mesh.faces.size()/3));

//     while(true){

//         angle += 0.5;
//         spin += rot_speed;

//         // auto mat_cam_rot = glm::rotate(glm::radians(angle), glm::vec3(1, 0, 0));
//         // glm::vec3 cam_location_vec = glm::vec3(0,0,camera_distance);
//         // glm::vec3 cam_pointing_vec = mat_cam_rot * glm::vec4(0, 0, -1, 0);
//         // glm::vec3 cam_up_vec = mat_cam_rot * glm::vec4(0, 1, 0, 0);

//         auto mat_vp = glm::perspective(glm::radians(70.f), 320.f/240.f, 0.001f, 1000.f)
//                     * glm::lookAt(glm::vec3(0, 0, camera_distance), glm::vec3(0, 0, camera_distance-1), glm::vec3(0, 1, 0));

//         auto mat_model = glm::rotate(glm::radians(angle),glm::vec3(0,1,0))
//                        * glm::rotate(glm::radians(spin),glm::vec3(1,1,1))
//                        * glm::translate(-show_object_mesh.center);
        
//         show_object_mesh.add_to_frame(display, mat_model, mat_vp, glm::vec3(0, 0, -1), glm::vec3(0, 0, camera_distance), glm::vec3(0, -1, 0));

//         display.draw_frame();

//         // return 0;

//         if (display.cur_frame_id % 60 == 0) {  // only update once a second
//             logging::info("Stats", "FPS: " + std::to_string(get_fps(display)) + " | Frame: " + std::to_string(display.cur_frame_id));
//             logging::info("Angle", std::to_string(angle));
//         }
//     }

//     return 0;
// }

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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/transform.hpp>

#include <cstring>
#include <vector>
#include <algorithm>
#include <memory>

extern Mesh show_object_mesh;

int show_object(){

    logging::info("ShowObject", "launching!");
    Display display(PIXEL_BUF_CTRL_BASE);

    int camera_distance = 15;

    auto mat_vp = glm::perspective(glm::radians(70.f), 320.f/240.f, 0.001f, 1000.f)
                * glm::lookAt(glm::vec3(0,0,camera_distance), glm::vec3(0,0,camera_distance-1), glm::vec3(0,1,0));

    init_timer_isr();
    init_random();
    float spin = 0;
    float angle = 0;
    float rot_speed = 0.5;

    logging::info("ShowObject", "# Triangles: " + std::to_string(show_object_mesh.faces.size()/3));

    while(true){
        angle += 0.5;
        spin += rot_speed;
        auto mat_model = glm::rotate(glm::radians(angle),glm::vec3(0,1,0))
                       * glm::rotate(glm::radians(spin),glm::vec3(1,1,1))
                       * glm::translate(-show_object_mesh.center);

        show_object_mesh.add_to_frame(display, mat_model, mat_vp, glm::vec3(0, 0, -1), glm::vec3(0, 0, camera_distance), glm::vec3(0, -1, 0));

        display.draw_frame();

        if (display.cur_frame_id % 60 == 0)    // only update once a second
            logging::info("Stats", "FPS: " + std::to_string(get_fps(display)) + " | Frame: " + std::to_string(display.cur_frame_id));
    }

    return 0;
}

