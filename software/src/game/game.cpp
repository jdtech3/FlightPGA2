// This serves as the main entrypoint of the game

#include "game/game.hpp"

#include "input/keyboard.hpp"
#include "input/PS2.hpp"

#include <stdexcept>

Game::Game(game_options_t game_options, enabled_hardware_t enabled_hardware) :
    game_options_(game_options),
    enabled_hardware_(enabled_hardware)
{
    logging::info("FlightGPA2", "launching...");

    display_ = std::make_unique<Display>(PIXEL_BUF_CTRL_BASE);
    char_buf_ = std::make_unique<CharBuf>(FPGA_CHAR_BASE);

    init_random();
    init_timer_isr();
    audio::init_isr();
    hex_display::init();

    
    

    if (enabled_hardware_.joystick)
        joystick_ = std::make_unique<Joystick>();
    if (enabled_hardware_.ps2_keyboard)
        ;   // TODO: implement
    if (enabled_hardware_.ps2_mouse)
        init_mouse_isr();

    plane_ = std::make_unique<Plane>();

    logging::ok("FlightGPA2", "launch complete!");
}

// int Game::run() {
//     while (true) {
//         plane_->update(0, 0.05, 0, 100);

//         display_->add_display_obj(Rectangle(100, 100, 100, 100, 0xFFFF));
//         display_->draw_frame();

//         if (display_->cur_frame_id % 20 == 0) {
//             char_buf_->eraseln(char_buf_->CHAR_BUF_HEIGHT - 1);
//             char_buf_->println(plane_->info_str(), char_buf_->CHAR_BUF_HEIGHT - 1);
//         }

//         if(display_->cur_frame_id % 60 == 0) {
//             if (game_options_.show_fps) {
//                 std::ostringstream os;
//                 os << "FlightGPA2 | FPS: " << get_fps(*display_) << " | Frame: " << display_->cur_frame_id;

//                 std::string str = os.str();
//                 char_buf_->eraseln();
//                 char_buf_->println(str);
//                 logging::info("Perf", str);
//             }

//             if (game_options_.debug_mode) {
//                 logging::info("Plane state", plane_->info_str(true, true));
//             }
//         }

//         if (game_options_.profiling_mode && display_->cur_frame_id == 3600) break;
//     }

//     return 0;
// }

void do_ps2(){
    // while(true){
        // u32 data = *reinterpret_cast<u32*>(ps2_1);
        // u32 rvalid = (data >> 15) & 1;
        // if(!rvalid) return;
        // u8 c;
        // u32 rvalid = ps2_1->read(c);
        // if(!rvalid) return;
        // else std::cout << std::hex << static_cast<int>(c) << std::endl;
    // }
    ps2_1->clear_fifo();
}

int Game::run(){

    float camera_distance = 150;
    glm::vec3 light_dir = glm::normalize(glm::vec3(1, 0, -1));

    while(true){
        check_reset();


        do_ps2();

        joystick_->update();
        // keyboard.poll();
        // if(keyboard.is_pressed(0x1C)) std::cout << "A pressed!" << std::endl;
        // std::cout << "R: " << joystick.roll << " P: " << joystick.pitch << " T%: " << joystick.throttle << std::endl;
        int status = plane_->update(joystick_->roll*10.f, joystick_->pitch*10.f, -plane_->get_roll()*0.1f, joystick_->throttle*50.f+50.f);
        if (status == -1) {
            char_buf_->println("YOU CRASHED! :(", 30, 35);
            audio::stop();
            break;
        }
        
        audio::set_volume(plane_->get_engine_power_percent());

        // Airspeed indicator overlay
        float airspeed_angle = ((plane_->get_speed() * constants::M_PER_SEC_TO_KNOTS) / 200.f) * 340.f;     // 200 kts at 340 deg ish
        glm::mat4 airspd_arrow_transform = glm::translate(glm::vec3(constants::PIXEL_BUF_WIDTH-70/2, 70/2, 0)) 
                                         * glm::rotate(glm::radians(airspeed_angle), glm::vec3(0, 0, 1));

        glm::vec3 airspd_arrow_v1 = airspd_arrow_transform * glm::vec4(-2, 0, 0, 1);
        glm::vec3 airspd_arrow_v2 = airspd_arrow_transform * glm::vec4(2, 0, 0, 1);
        glm::vec3 airspd_arrow_v3 = airspd_arrow_transform * glm::vec4(0, -20, 0, 1);
        display_->add_display_obj(
            Triangle(
                airspd_arrow_v1.x, airspd_arrow_v1.y,
                airspd_arrow_v2.x, airspd_arrow_v2.y,
                airspd_arrow_v3.x, airspd_arrow_v3.y,
                0xFFFF
            )
        );
        display_->add_display_obj(Image(reinterpret_cast<const u16*>(assets::airspeed_indicator), constants::PIXEL_BUF_WIDTH-71, 0, 70, 70));

        auto plane_heading = plane_->get_heading();
        plane_heading = glm::vec3(plane_heading.x, plane_heading.y, 0);

        Camera camera = {
            plane_->get_pos() - camera_distance*plane_heading + glm::vec3(0,0,camera_distance/5.f),
            plane_heading,
            plane_->get_up()
        };

        glm::mat4 mat_plane_model = glm::translate(glm::vec3(plane_->get_pos()))
                                  * glm::rotate(glm::radians(plane_->get_yaw()), glm::vec3(0,0,1))
                                  * glm::rotate(glm::radians(plane_->get_pitch()), glm::vec3(1,0,0))
                                  * glm::rotate(glm::radians(plane_->get_roll()), glm::vec3(0,1,0))
                                  * glm::scale(glm::vec3(10, 10, 10))
                                  * glm::mat4(0, 1, 0, 0, 0, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 1)
                                  * glm::translate(-assets::airplane_mesh.center);

        assets::airplane_mesh.add_to_frame(*display_, mat_plane_model, camera, light_dir);
        draw_trees(camera, light_dir);
        draw_ground(camera, light_dir);

        if (display_->cur_frame_id == 2) {
            // We're launching, play some sounds
            audio::play(assets::seatbelt_sound, assets::seatbelt_sound_length, false);
            while (!audio::done());     // block until done
            
            audio::play(assets::engine_sound, assets::engine_sound_length);
        }

        display_->draw_frame();

        if (display_->cur_frame_id % 60 == 0) {  // only update once a second
            logging::info("Plane", plane_->info_str(true, true));
            
            if (game_options_.show_fps) {
                std::ostringstream os;
                os << "FlightGPA2 | FPS: " << get_fps(*display_) << " | Frame: " << display_->cur_frame_id;

                std::string str = os.str();
                char_buf_->eraseln();
                char_buf_->println(str);
                logging::info("Perf", str);
            }
        }

        if (display_->cur_frame_id % 10 == 0) {
            char_buf_->eraseln(char_buf_->CHAR_BUF_HEIGHT - 1);
            char_buf_->println(plane_->info_str(), char_buf_->CHAR_BUF_HEIGHT - 1);
            
            char_buf_->println("^", 6, 39);
            char_buf_->eraseln(8);
            std::ostringstream os;
            int principal_yaw = static_cast<int>(std::round(plane_->get_yaw())) % 360;
            int heading = std::round(principal_yaw > 0 ? 360 - principal_yaw : -principal_yaw);
            os << "-  " << std::setw(3) << std::setfill('0') << static_cast<int>(heading) << std::setfill(' ') << "  -";
            char_buf_->println(os.str(), 8, 35);

            hex_display::display_uint(static_cast<u32>(plane_->get_pos().z));
        }
    }

    return 0;
}

void Game::draw_trees(const Camera& camera, const glm::vec3& light_dir){
    
    float x_closest = std::roundf(camera.pos.x/constants::TREE_SPACING)*constants::TREE_SPACING;
    float y_closest = std::roundf(camera.pos.y/constants::TREE_SPACING)*constants::TREE_SPACING;


    for(float s = 0; s <= constants::TREE_RENDER_DIST; s+=constants::TREE_SPACING){
        float x_min = x_closest-s;
        float y_min = y_closest-s;
        float x_max = x_closest+s;
        float y_max = y_closest+s;

        auto translate_and_draw = [&](float x, float y){
            glm::vec3 loc = glm::vec3(x,y,0);
            glm::vec3 closest = glm::vec3(x_closest, y_closest, 0);
            if(glm::dot(loc-closest, camera.dir) <= 0) return;
            glm::mat4 mat_model = glm::translate(loc);
            assets::tree_mesh.add_to_frame(*display_, mat_model, camera, light_dir);
        };

        for(float x = x_min; x <= x_max; x+=constants::TREE_SPACING){
            int y = y_min;
            translate_and_draw(x,y);
            y = y_max;
            translate_and_draw(x,y);
        }

        for(float y = y_min; y <= y_max; y+=constants::TREE_SPACING){
            int x = x_min;
            translate_and_draw(x,y);
            x = x_max;
            translate_and_draw(x,y);
        }


    }
}

void Game::draw_ground(const Camera& camera, const glm::vec3& light_dir){
    glm::vec2 camera_dir_projected = glm::normalize(glm::vec2(camera.dir.x, camera.dir.y));
    glm::vec3 camera_pos_projected = glm::vec3(camera.pos.x, camera.pos.y, 0);
    float angle = glm::orientedAngle(glm::vec2(0,1), camera_dir_projected);
    // std::cout << "angle: " << angle << std::endl;
    for(int i = 5; i < constants::TREE_RENDER_DIST/constants::GND_SIZE_FORWARD; i++){
        glm::mat4 model = glm::translate(camera_pos_projected)
                        * glm::rotate(angle, glm::vec3(0,0,1))
                        * glm::translate(glm::vec3(0, constants::GND_SIZE_FORWARD*i, 0));
        assets::ground_strip_mesh.add_to_frame(*display_, model, camera, light_dir);
    }

}
