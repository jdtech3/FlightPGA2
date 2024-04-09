#pragma once

#include <cmath>
#include <sstream>
#include <iomanip>

#define GLM_ENABLE_EXPERIMENTAL
#include "glm/vec3.hpp"
#include "glm/vec4.hpp"
#include "glm/mat4x4.hpp"
#include "glm/trigonometric.hpp"
#include "glm/geometric.hpp"
#include "glm/gtx/transform.hpp"
#include "glm/gtx/vector_angle.hpp"
#include "glm/gtx/string_cast.hpp"

#include "utils/logging.hpp"
#include "utils/clock.hpp"
#include "cstdint_short.hpp"
#include "constants.hpp"


typedef enum plane_state {
    LANDED,
    FLYING,
    CRASHED,
} plane_state_t;

typedef enum flaps_settings {
    FLAPS_NONE  = 0,
    FLAPS_10    = 10,
    FLAPS_20    = 20,
    FLAPS_FULL  = 30,
} flaps_settings_t;

class Plane {
    private:
        // -- Model constants

        // Following constants based on Cessna 172
        constexpr static float WING_AREA = 16.17f;          // m^2
        constexpr static float DRY_WEIGHT = 706.4f;         // kg, incl. 70 kg pilot
        constexpr static float MAX_FUEL = 162.8f;           // L
        constexpr static float STALL_SPEED = 25.47f;        // m/s (91.7 km/h)
        constexpr static float STALL_SPEED_FLAPS = 22.81f;  // m/s (82.1 km/h)
        constexpr static float MAX_SPEED = 77.78f;          // m/s (280 km/h)
        constexpr static float MAX_ENGINE_POWER = 250.f * constants::g;                 // kg m/s^2 = N
        constexpr static float ENGINE_POWER_PER_PERCENT = MAX_ENGINE_POWER / 100.f;     // kg m/s^2 = N

        // Following constants are arbitrarily chosen
        constexpr static float LANDING_GEAR_HEIGHT = 0.3f;          // m
        constexpr static float LANDING_VERTICAL_SPEED_MAX = 0.5f;   // m/s
        constexpr static float FLAPS_AOA_INCREASE_COEF = 0.1f;      // deg AOA/deg flaps
        constexpr static float FUEL_BURN_RATE = 0.1f;               // L/sec (real Cessna is 0.0095, but that'll take hours to see fuel effects)
        constexpr static float ENGINE_POWER_SLEW_RATE = 10.f * ENGINE_POWER_PER_PERCENT;    // N/sec
        constexpr static float DRAG_MULTIPLIER = 5.f;

        // -- State

        float last_update_s_;    // time of last update

        // Orientation - raw
        float roll_;        // deg, +ve is roll right
        float pitch_;       // deg, +ve is pitch up
        float yaw_;         // deg, +ve is counterclockwise viewed from top

        // Orientation - derived
        glm::mat4 rot_matrix_;
        glm::vec3 heading_;     // unit vec
        glm::vec3 up_;          // unit vec
        glm::vec3 right_;       // unit vec

        // Kinematics
        glm::vec3 pos_;         // <x, y, z> m
        glm::vec3 vel_;         // <x, y, z> m/s
        glm::vec3 accel_;       // <x, y, z> m/s^2

        // System
        float fuel_;                // L
        float engine_power_;        // N
        flaps_settings_t flaps_;

        // -- Helper functions

        // Aerodynamics
        static float lift_coef_(float aoa);
        static float drag_coef_(float aoa);
        static float lift_(float aoa, float vel);
        static float drag_(float aoa, float vel);

        // Orientation
        void update_orientation_tensors_();

    public:
        Plane();

        plane_state_t state;

        // State update
        int update(float roll_rate, float pitch_rate, float yaw_rate, float throttle); // rates in deg/s, throttle in %
        void reset_position();
        flaps_settings_t increase_flaps();
        flaps_settings_t decrease_flaps();

        // Aerodynamics
        float cur_aoa() const;
        float cur_aoa(float heading_dot_vel) const;

        // Kinematics
        float cur_mass() const;
        glm::vec3 cur_net_force() const;

        // Utility
        glm::vec3 get_pos() const;
        glm::vec3 get_heading() const;
        glm::vec3 get_up() const;
        float get_speed() const;
        float get_roll() const;
        float get_pitch() const;
        float get_yaw() const;
        float get_engine_power_percent() const;
        std::string info_str(bool uart = false, bool debug = false) const;
        operator std::string() const;
};
