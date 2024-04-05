#include "game/plane.hpp"

// -- Constructor

Plane::Plane() :
    last_update_s_(get_clock_s()),
    roll_(0), pitch_(0), yaw_(0),
    pos_(glm::vec3(0, 0, LANDING_GEAR_HEIGHT)), vel_(glm::vec3(0)), accel_(glm::vec3(0)),
    fuel_(MAX_FUEL), engine_power_(0), flaps_(FLAPS_NONE),
    state(LANDED)
{
    update_orientation_tensors_();
}

// -- Private

// Based on 6th order polynomial approximation of NACA 2412 (Cessna 172's airfoil)
// Raw data from: http://airfoiltools.com/polar/details?polar=xf-naca2412-il-1000000-n5
float Plane::lift_coef_(float aoa) {
    return 0.241 + 0.106*aoa + 1.82e-4*std::pow(aoa, 2) + 6.17e-5*std::pow(aoa, 3) +
           -7.8e-6*std::pow(aoa, 4) + -4.77e-7*std::pow(aoa, 5) + 2.12e-8*std::pow(aoa, 6);
}
float Plane::drag_coef_(float aoa) {
    return 5.81e-3 + -3.96e-4*aoa + 1.85e-4*std::pow(aoa, 2) + 8.36e-6*std::pow(aoa, 3) +
           -1.38e-6*std::pow(aoa, 4) + -2.63e-8*std::pow(aoa, 5) + 4.41e-9*std::pow(aoa, 6);
}

// Aero equations
float Plane::lift_(float aoa, float vel) {
    return 0.5f * Plane::lift_coef_(aoa) * constants::RHO_AIR * std::pow(vel, 2) * WING_AREA;
}
float Plane::drag_(float aoa, float vel) {
    return 0.5f * Plane::drag_coef_(aoa) * constants::RHO_AIR * std::pow(vel, 2) * WING_AREA;  // using wing area as ref area here
}

// Utility
void Plane::update_orientation_tensors_() {
    glm::mat4 roll_matrix = glm::rotate(glm::radians(roll_), glm::vec3(0, 1, 0));
    glm::mat4 pitch_matrix = glm::rotate(glm::radians(pitch_), glm::vec3(1, 0, 0));
    glm::mat4 yaw_matrix = glm::rotate(glm::radians(yaw_), glm::vec3(0, 0, 1));
    rot_matrix_ = yaw_matrix * pitch_matrix * roll_matrix;   // applied in order of roll, then pitch, then yaw

    heading_ = glm::vec3(rot_matrix_ * glm::vec4(0, 1, 0, 0));
    up_ = glm::vec3(rot_matrix_ * glm::vec4(0, 0, 1, 0));
}

// -- Public

// ! NOTE: not sure if this gives correct values
float Plane::cur_aoa() const {
    return glm::degrees(glm::acos(glm::dot(heading_, vel_) / (glm::length(vel_))));   // heading is unit vec
}
float Plane::cur_aoa(float heading_dot_vel) const {                     // for optimization
    return glm::degrees(glm::acos(heading_dot_vel / (glm::length(vel_))));
}

float Plane::cur_mass() const { return DRY_WEIGHT + fuel_; }

glm::vec3 Plane::cur_net_force() const {
    float heading_dot_vel = glm::dot(heading_, vel_);

    glm::vec3 weight(0, 0, -cur_mass() * constants::g);
    glm::vec3 thrust = heading_ * engine_power_ * ENGINE_POWER_PER_PERCENT;
    glm::vec3 lift = up_ * lift_(cur_aoa(heading_dot_vel), heading_dot_vel);
    glm::vec3 drag = -heading_ * drag_(cur_aoa(heading_dot_vel), heading_dot_vel);

    return weight + thrust + lift + drag;
}

// State update
void Plane::update(float roll_rate, float pitch_rate, float yaw_rate, float throttle) {
    // Update time
    float current_time = get_clock_s();
    float time_passed = current_time - last_update_s_;
    last_update_s_ = current_time;

    // Update kinematics
    // ? Which order is better here, update accel -> vel -> pos, or pos -> vel -> accel (i.e. using last)
    accel_ = cur_net_force() / cur_mass();
    vel_ += accel_ * time_passed;
    pos_ += vel_ * time_passed;

    // Update orientation
    roll_ += roll_rate * time_passed;
    pitch_ += pitch_rate * time_passed;
    yaw_ += yaw_rate * time_passed;
    update_orientation_tensors_();

    // Update system
    fuel_ -= time_passed * FUEL_BURN_RATE;

    float req_engine_power = throttle * ENGINE_POWER_PER_PERCENT;
    if (fabsf(req_engine_power - engine_power_) > ENGINE_POWER_SLEW_RATE)
        engine_power_ += ENGINE_POWER_SLEW_RATE;
    else
        engine_power_ = req_engine_power;

    // Update state
    if (pos_.z < LANDING_GEAR_HEIGHT) {
        pos_.z = LANDING_GEAR_HEIGHT;
        state = (vel_.z < -LANDING_VERTICAL_SPEED_MAX) ? CRASHED : LANDED;
    }
    else
        state = FLYING;
}

void Plane::reset_position() {
    last_update_s_ = get_clock_s();
    roll_ = 0;
    pitch_ = 0;
    yaw_ = 0;
    pos_ = glm::vec3(0);
    vel_ = glm::vec3(0);
    accel_ = glm::vec3(0);

    update_orientation_tensors_();
}

flaps_settings_t Plane::increase_flaps() {
    if (flaps_ != FLAPS_FULL) flaps_ = static_cast<flaps_settings_t>(static_cast<int>(flaps_) + 10);
    return flaps_;
}
flaps_settings_t Plane::decrease_flaps() {
    if (flaps_ != FLAPS_NONE) flaps_ = static_cast<flaps_settings_t>(static_cast<int>(flaps_) - 10);
    return flaps_;
};

Plane::operator std::string() const {
    std::ostringstream os;
    os << "State: " << ((state == LANDED) ? "Landed" : ((state == FLYING) ? "Flying" : "Crashed"));
    os << " Roll: " << roll_ << "° Pitch: " << pitch_ << "° Yaw: " << yaw_ << "°";
    os << " Heading vec: " << glm::to_string(heading_) << " Up vec: " << glm::to_string(up_);
    os << " Pos: " << glm::to_string(pos_) << " Vel: " << glm::to_string(vel_) << " Accel: " << glm::to_string(accel_);
    os << " Fuel: " << fuel_ << "L Engine power: " << engine_power_ << "N Flaps: " << flaps_ << "°";
    return os.str();
}