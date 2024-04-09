
#include "game/plane.hpp"

// -- Constructor

Plane::Plane() :
    last_update_s_(get_clock_s()),
    roll_(0), pitch_(0), yaw_(glm::radians(0.f)),
    pos_(glm::vec3(constants::TREE_SPACING/2, 0, LANDING_GEAR_HEIGHT)), vel_(0), accel_(0),
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
    right_ = glm::vec3(rot_matrix_ * glm::vec4(1, 0, 0, 0));
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
    float aoa = glm::orientedAngle(vel_, heading_, right_);

    glm::vec3 weight(0, 0, -cur_mass() * constants::g);
    glm::vec3 thrust = heading_ * engine_power_;
    glm::vec3 lift = up_ * lift_(aoa, heading_dot_vel);
    // glm::vec3 drag = -heading_ * drag_(aoa, heading_dot_vel) * DRAG_MULTIPLIER;
    glm::vec3 drag = -((vel_ == glm::vec3(0,0,0)) ? glm::vec3(0,0,0) : glm::normalize(vel_)) * drag_(aoa, heading_dot_vel) * 5.f;

    // glm::vec3 drag_vert = up_ * drag_(0, glm::dot(up_, vel_)) * 100.f;

    // temp clamp
    // if (glm::length2(lift) > glm::length2(weight)) lift = glm::normalize(lift) * glm::length(weight);
    if (lift.z > -weight.z) lift = fabsf(weight.z / lift.z) * lift;

    // logging::info("Angle of attack", std::to_string(aoa));
    // logging::info("Lift", glm::to_string(lift));
    // logging::info("Drag", glm::to_string(drag));

    return thrust + drag;
    // return weight + thrust + lift + drag;
    // return weight + thrust + lift + drag + drag_vert;
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
    if ((req_engine_power - engine_power_) > (ENGINE_POWER_SLEW_RATE * time_passed))
        engine_power_ += ENGINE_POWER_SLEW_RATE * time_passed;
    else if ((engine_power_ - req_engine_power) > (ENGINE_POWER_SLEW_RATE * time_passed))
        engine_power_ -= ENGINE_POWER_SLEW_RATE * time_passed;
    else
        engine_power_ = req_engine_power;

    // Update state
    switch (state) {
        case LANDED:
            if (pos_.z > LANDING_GEAR_HEIGHT)
                state = FLYING;
            else
                pos_.z = LANDING_GEAR_HEIGHT;   // clamp

            if (vel_.z < 0) vel_.z = 0;         // clamp
            break;
        case CRASHED:
            if (pos_.z <= LANDING_GEAR_HEIGHT) {
                pos_.z = LANDING_GEAR_HEIGHT;
                vel_.z = 0.f;
            }
        case FLYING:
            if (pos_.z <= LANDING_GEAR_HEIGHT)
                state = (vel_.z < -LANDING_VERTICAL_SPEED_MAX) ? CRASHED : LANDED;
            break;
    }
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

glm::vec3 Plane::get_pos() const { return pos_; }
glm::vec3 Plane::get_heading() const { return heading_; }
glm::vec3 Plane::get_up() const { return up_; }
float Plane::get_speed() const { return glm::length(vel_); }
float Plane::get_roll() const { return roll_; }
float Plane::get_pitch() const { return pitch_; }
float Plane::get_yaw() const { return yaw_; }

float Plane::get_engine_power_percent() const { return engine_power_ / MAX_ENGINE_POWER; };

std::string Plane::info_str(bool uart, bool debug) const {
    std::ostringstream os;
    os << std::fixed << std::setprecision(0);
    os << ((state == LANDED) ? "LANDED" : ((state == FLYING) ? "FLYING" : "CRASHED"));
    if (uart) os << " Roll: " << roll_ << "° Pitch: " << pitch_ << "° Yaw: " << yaw_ << "°";
    os << " Speed: " << glm::length(vel_) * constants::M_PER_SEC_TO_KNOTS << " kts";
    os << " Fuel: " << fuel_ << "L Thrust: " << engine_power_ << "N Flaps: " << flaps_ << (uart ? "°" : "");

    if (debug) {
        os << "\nHeading vec: " << glm::to_string(heading_) << " Up vec: " << glm::to_string(up_) << " Right vec: " << glm::to_string(right_);
        os << "\nPos vec: " << glm::to_string(pos_) << " Vel vec: " << glm::to_string(vel_) << " Accel vec: " << glm::to_string(accel_);
    }

    return os.str();
}

Plane::operator std::string() const {
    return info_str();
}
