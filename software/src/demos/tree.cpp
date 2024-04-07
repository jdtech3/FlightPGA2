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

const Mesh tree_mesh = {
    {0, 0, 0},
    {
        {0.f, TREE_BUSH_HEIGHT+TREE_TRUNK_HEIGHT, 0.f},
        {-TREE_BUSH_SIDE, TREE_TRUNK_HEIGHT, -TREE_BUSH_SIDE},
        {-TREE_BUSH_SIDE, TREE_TRUNK_HEIGHT, +TREE_BUSH_SIDE},
        {+TREE_BUSH_SIDE, TREE_TRUNK_HEIGHT, +TREE_BUSH_SIDE},
        {+TREE_BUSH_SIDE, TREE_TRUNK_HEIGHT, -TREE_BUSH_SIDE},
        {-TREE_TRUNK_SIDE, TREE_TRUNK_HEIGHT, -TREE_TRUNK_SIDE},
        {-TREE_TRUNK_SIDE, TREE_TRUNK_HEIGHT, +TREE_TRUNK_SIDE},
        {+TREE_TRUNK_SIDE, TREE_TRUNK_HEIGHT, +TREE_TRUNK_SIDE},
        {+TREE_TRUNK_SIDE, TREE_TRUNK_HEIGHT, -TREE_TRUNK_SIDE},
        {-TREE_TRUNK_SIDE, 0.f, -TREE_TRUNK_SIDE},
        {-TREE_TRUNK_SIDE, 0.f, +TREE_TRUNK_SIDE},
        {+TREE_TRUNK_SIDE, 0.f, +TREE_TRUNK_SIDE},
        {+TREE_TRUNK_SIDE, 0.f, -TREE_TRUNK_SIDE},
    },
    {
        // Tree Bush
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1,
        // Tree Trunk
        5, 6, 9,
        6, 9, 10,
        6, 7, 10,
        7, 10, 11,
        11, 7, 12,
        7, 12, 8,
        8, 12, 5,
        12, 5, 9
    },
    {
        // Tree Bush
        glm::normalize(glm::vec3(-TREE_BUSH_HEIGHT, TREE_BUSH_SIDE, 0.f)),
        glm::normalize(glm::vec3(0.f, TREE_BUSH_SIDE, TREE_BUSH_HEIGHT)),
        glm::normalize(glm::vec3(TREE_BUSH_HEIGHT, TREE_BUSH_SIDE, 0.f)),
        glm::normalize(glm::vec3(0.f, TREE_BUSH_SIDE, -TREE_BUSH_HEIGHT)),
        // Tree Trunk
        {-1.f, 0.f, 0.f},
        {-1.f, 0.f, 0.f},
        {0.f, 0.f, 1.f},
        {0.f, 0.f, 1.f},
        {1.f, 0.f, 0.f},
        {1.f, 0.f, 0.f},
        {0.f, 0.f, -1.f},
        {0.f, 0.f, -1.f}
    },
    {
        // Tree Bush
        TREE_BUSH_COLOR,
        TREE_BUSH_COLOR,
        TREE_BUSH_COLOR,
        TREE_BUSH_COLOR,
        // Tree Trunk
        TREE_TRUNK_COLOR,
        TREE_TRUNK_COLOR,
        TREE_TRUNK_COLOR,
        TREE_TRUNK_COLOR,
        TREE_TRUNK_COLOR,
        TREE_TRUNK_COLOR,
        TREE_TRUNK_COLOR,
        TREE_TRUNK_COLOR
    }
};

extern Mesh show_object_mesh;

int tree(){

    logging::info("Tree", "launching!");
    Display display(PIXEL_BUF_CTRL_BASE);

    int camera_distance = 200;

    init_timer_isr();
    init_random();

    float angle = 0;

    while(true){
        angle += 0.05;

        auto mat_vp = glm::perspective(glm::radians(70.f), 320.f/240.f, 0.001f, 1000.f)
                    * glm::lookAt(glm::vec3(0, 50, camera_distance), glm::vec3(0,50,camera_distance-1), glm::vec3(0,1,0));
        
        auto mat_model = glm::rotate(angle, glm::vec3(0, 1, 0))
                       * glm::translate(-tree_mesh.center);

        tree_mesh.add_to_frame(display, mat_model, mat_vp, glm::vec3(0, 0, -1), glm::vec3(0, 0, 5), glm::normalize(glm::vec3(1, -1, 0)));

        display.draw_frame();

        if (display.cur_frame_id % 60 == 0) {  // only update once a second
            logging::info("Stats", "FPS: " + std::to_string(get_fps(display)) + " | Frame: " + std::to_string(display.cur_frame_id));
        }
    }

    return 0;
}


