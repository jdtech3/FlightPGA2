
#include "demos/demos.hpp"

#include "address_map_nios2.h"
#include "constants.h"
#include "display/display.hpp"
#include "display/primitives.hpp"
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

    const std::array<HighTri, 2*6> cube_vertices{ // 2 tirangles per face * 6 faces per cube
        HighTri(glm::vec3(-.5f,-.5f,-.5f),glm::vec3(-.5f,+.5f,-.5f),glm::vec3(+.5f,-.5f,-.5f),0xF800), // back face
        HighTri(glm::vec3(+.5f,+.5f,-.5f),glm::vec3(-.5f,+.5f,-.5f),glm::vec3(+.5f,-.5f,-.5f),0xF800), // ^
        HighTri(glm::vec3(-.5f,-.5f,-.5f),glm::vec3(+.5f,-.5f,-.5f),glm::vec3(+.5f,-.5f,+.5f),0x07E0), // bottom face
        HighTri(glm::vec3(-.5f,-.5f,-.5f),glm::vec3(-.5f,-.5f,+.5f),glm::vec3(+.5f,-.5f,+.5f),0x07E0), // ^
        HighTri(glm::vec3(+.5f,+.5f,+.5f),glm::vec3(-.5f,+.5f,+.5f),glm::vec3(+.5f,-.5f,+.5f),0xF800), // front face
        HighTri(glm::vec3(-.5f,-.5f,+.5f),glm::vec3(-.5f,+.5f,+.5f),glm::vec3(+.5f,-.5f,+.5f),0xF800), // ^
        HighTri(glm::vec3(-.5f,+.5f,-.5f),glm::vec3(+.5f,+.5f,-.5f),glm::vec3(+.5f,+.5f,+.5f),0x07E0), // top face
        HighTri(glm::vec3(-.5f,+.5f,-.5f),glm::vec3(-.5f,+.5f,+.5f),glm::vec3(+.5f,+.5f,+.5f),0x07E0), // ^
        HighTri(glm::vec3(-.5f,-.5f,-.5f),glm::vec3(-.5f,+.5f,-.5f),glm::vec3(-.5f,-.5f,+.5f),0x001F), // left face
        HighTri(glm::vec3(-.5f,+.5f,+.5f),glm::vec3(-.5f,+.5f,-.5f),glm::vec3(-.5f,-.5f,+.5f),0x001F), // ^
        HighTri(glm::vec3(+.5f,-.5f,-.5f),glm::vec3(+.5f,+.5f,-.5f),glm::vec3(+.5f,-.5f,+.5f),0x001F), // right face
        HighTri(glm::vec3(+.5f,+.5f,+.5f),glm::vec3(+.5f,+.5f,-.5f),glm::vec3(+.5f,-.5f,+.5f),0x001F), // ^
    };

    auto mat_vp = glm::perspective(glm::radians(70.f), 320.f/240.f, 0.001f, 1000.f)
                * glm::lookAt(glm::vec3(0,0,5), glm::vec3(0,0,4), glm::vec3(0,1,0));

    init_timer_isr();
    init_random();
    float angle = 0;

    while(true){
        angle += static_cast<float>(randint(100, 5000)) / 1000.0f;
        auto mat_model = glm::rotate(glm::radians(angle),glm::vec3(1,1,1));
        auto mat_mvp = mat_vp*mat_model;

        std::vector<std::unique_ptr<HighTri>> after_mvp;
        for(const auto& tri : cube_vertices){
            after_mvp.push_back(std::make_unique<HighTri>(
                mat_mvp*glm::vec4(tri.v1,1.f),
                mat_mvp*glm::vec4(tri.v2,1.f),
                mat_mvp*glm::vec4(tri.v3,1.f),
                tri.color
            ));
        }
        std::sort(after_mvp.begin(), after_mvp.end(), HighTri::sort_depth_);
        // logging::info("After MVP size", std::to_string(after_mvp.size()));
        for(const auto& tri : after_mvp){
            Triangle disp_obj = Triangle(
                static_cast<u16>((320.f/2)*tri->v1.x/tri->v1.z + 320.f/2), static_cast<u16>((-240.f/2)*tri->v1.y/tri->v1.z + 240.f/2),
                static_cast<u16>((320.f/2)*tri->v2.x/tri->v2.z + 320.f/2), static_cast<u16>((-240.f/2)*tri->v2.y/tri->v2.z + 240.f/2),
                static_cast<u16>((320.f/2)*tri->v3.x/tri->v3.z + 320.f/2), static_cast<u16>((-240.f/2)*tri->v3.y/tri->v3.z + 240.f/2),
                tri->color);
            // std::cout << disp_obj << std::endl;
            display.add_display_obj(std::move(disp_obj));
        }
        display.draw_frame();

        if (display.cur_frame_id % 60 == 0)    // only update once a second
            logging::info("Stats", "FPS: " + std::to_string(get_fps(display)) + " | Frame: " + std::to_string(display.cur_frame_id));
    }

    return 0;
}