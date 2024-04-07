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

    if (enabled_hardware_.joystick)
        joystick_ = std::make_unique<Joystick>();
    if (enabled_hardware_.ps2_keyboard)
        ;   // TODO: implement
    if (enabled_hardware_.ps2_mouse)
        init_mouse_isr();

    plane_ = std::make_unique<Plane>();

    logging::ok("FlightGPA2", "launch complete!");
}

int Game::run() {
    while (true) {
        plane_->update(0, 0.05, 0, 100);

        display_->add_display_obj(Rectangle(100, 100, 100, 100, 0xFFFF));
        display_->draw_frame();

        if (display_->cur_frame_id % 20 == 0) {
            char_buf_->eraseln(char_buf_->CHAR_BUF_HEIGHT - 1);
            char_buf_->println(plane_->info_str(), char_buf_->CHAR_BUF_HEIGHT - 1);
        }

        if(display_->cur_frame_id % 60 == 0) {
            if (game_options_.show_fps) {
                std::ostringstream os;
                os << "FlightGPA2 | FPS: " << get_fps(*display_) << " | Frame: " << display_->cur_frame_id;
                
                std::string str = os.str();
                char_buf_->eraseln();
                char_buf_->println(str);
                logging::info("Perf", str);
            }

            if (game_options_.debug_mode) {
                logging::info("Plane state", plane_->info_str(true, true));
            }
        }

        if (game_options_.profiling_mode && display_->cur_frame_id == 3600) break;
    }

    return 0;
}
