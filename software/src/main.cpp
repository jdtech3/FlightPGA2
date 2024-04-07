#include "demos/demos.hpp"
#include "game/game.hpp"
#include "constants.hpp"

#define DEMO;

int main() {
    #ifdef DEMO
        return tree();
    #else
        game_options_t options = constants::default_game_options;
        options.debug_mode = true;

        Game game(options);
        game.run();
    #endif
}
