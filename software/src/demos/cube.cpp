
#include "demos/demos.hpp"

#include "address_map_nios2.h"
#include "constants.h"
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

#define min(x,y) ((x < y) ? x : y)
#define max(x,y) ((x > y) ? x : y)

struct HighTri{
    glm::vec3 v1,v2,v3;
    u16 color;
    HighTri(glm::vec3 v1, glm::vec3 v2, glm::vec3 v3, u16 color):
        v1(v1), v2(v2), v3(v3), color(color){}
    float depth() const {
        return v1.z + v2.z + v3.z;
    }
    static bool sort_depth_(const std::unique_ptr<HighTri>& a, const std::unique_ptr<HighTri>& b){
        return a->depth() < b->depth();
    }
};

int cube(){

    logging::info("CubeDemo", "launching!");
    Display display(PIXEL_BUF_CTRL_BASE);
    // display.clear();

    const Mesh cube_mesh(
        glm::vec3(0,0,0),
        {
            glm::vec3(-.5f,-.5f,-.5f), 
            glm::vec3(+.5f,-.5f,-.5f),
            glm::vec3(+.5f,+.5f,-.5f),
            glm::vec3(-.5f,+.5f,-.5f),
            glm::vec3(-.5f,-.5f,+.5f), 
            glm::vec3(+.5f,-.5f,+.5f),
            glm::vec3(+.5f,+.5f,+.5f),
            glm::vec3(-.5f,+.5f,+.5f)
        },
        {
            0, 1, 3, 1, 3, 2,   // back
            4, 5, 7, 7, 5, 6,   // front
            0, 3, 4, 3, 4, 7,   // left
            5, 1, 6, 1, 6, 2,   // right
            2, 3, 6, 3, 6, 7,   // top
            0, 1, 4, 1, 4, 5    // bottom
        },
        {
            glm::vec3(+0.f,+0.f,-1.f),
            glm::vec3(+0.f,+0.f,-1.f),
            glm::vec3(+0.f,+0.f,+1.f),
            glm::vec3(+0.f,+0.f,+1.f),
            glm::vec3(-1.f,+0.f,+0.f),
            glm::vec3(-1.f,+0.f,+0.f),
            glm::vec3(+1.f,+0.f,+0.f),
            glm::vec3(+1.f,+0.f,+0.f),
            glm::vec3(+0.f,+1.f,+0.f),
            glm::vec3(+0.f,+1.f,+0.f),
            glm::vec3(+0.f,-1.f,+0.f),
            glm::vec3(+0.f,-1.f,+0.f),
        },
        // {
        //     0xF800,0xF800,0xF800,0xF800,
        //     0x001F,0x001F,0x001F,0x001F,
        //     0x07E0,0x07E0,0x07E0,0x07E0
        // }
        {
            0xF800,0xF800,0xF800,0xF800,
            0xF800,0xF800,0xF800,0xF800,
            0xF800,0xF800,0xF800,0xF800
        }
    );

    auto mat_vp = glm::perspective(glm::radians(70.f), 320.f/240.f, 0.001f, 1000.f)
                * glm::lookAt(glm::vec3(0,0,5), glm::vec3(0,0,4), glm::vec3(0,1,0));

    init_timer_isr();
    init_random();
    float spin = 0;
    float angle = 0;
    float rot_speed = 0.5;

    while(true){
        // if(display.cur_frame_id % 60 == 0) rot_speed = static_cast<float>(randint(100, 5000)) / 1000.0f;
        angle += 0.5;
        spin += rot_speed;
        auto mat_model = glm::rotate(glm::radians(angle),glm::vec3(0,1,0))
                       * glm::rotate(glm::radians(spin),glm::vec3(1,1,1));

        cube_mesh.add_to_frame(display, mat_model, mat_vp, glm::vec3(0, 0, -1), glm::vec3(0, 0, 5), glm::vec3(0, -1, 0));

        display.draw_frame();

        if (display.cur_frame_id % 60 == 0)    // only update once a second
            logging::info("Stats", "FPS: " + std::to_string(get_fps(display)) + " | Frame: " + std::to_string(display.cur_frame_id));
    }

    return 0;
}