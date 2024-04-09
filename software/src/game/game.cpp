// This serves as the main entrypoint of the game

#include "game/game.hpp"

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

int Game::run(){

    float camera_distance = 150;
    glm::vec3 light_dir = glm::normalize(glm::vec3(1, 0, -1));

    audio::play(assets::engine_sound, assets::engine_sound_length, true);

    while(true){
        // angle += 0.05;
        joystick_->update();
        // std::cout << "R: " << joystick.roll << " P: " << joystick.pitch << " T%: " << joystick.throttle << std::endl;
        plane_->update(joystick_->roll*10.f, joystick_->pitch*10.f, -plane_->get_roll()*0.1f, joystick_->throttle*50.f+50.f);
        audio::set_volume(plane_->get_engine_power_percent());
        // if (plane.get_speed() < 60.f)
        //     plane.update(0,0,0,100);
        // else if (plane.get_pitch() < 10.f)
        //     plane.update(0,1,0,100);
        // else
        //     plane.update(0,0,0,100);

        display_->add_display_obj(Image(reinterpret_cast<const u16*>(assets::airspeed_indicator), 0, 0, 70, 70));

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

        display_->draw_frame();

        if (display_->cur_frame_id % 60 == 0) {  // only update once a second
            logging::info("Plane", plane_->info_str(true, true));
            logging::info("Stats", "FPS: " + std::to_string(get_fps(*display_)) + " | Frame: " + std::to_string(display_->cur_frame_id));
        }

        if (display_->cur_frame_id % 10 == 0) {
            char_buf_->eraseln(char_buf_->CHAR_BUF_HEIGHT - 1);
            char_buf_->println(plane_->info_str(), char_buf_->CHAR_BUF_HEIGHT - 1);
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
