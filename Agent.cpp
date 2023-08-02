//
// Created by igor on 14/06/2023.
//

#include "Agent.h"

Agent::Agent(const std::string &name_, Point location_, int speed_, int health_)
    : Sim_object(name_, location_), speed(speed_), health(health_),
      state(STOPPED) {}

void Agent::update() {
    if (state == MOVING_TO_POSITION || state == MOVING_ON_COURSE){
        setLocation(Point::advance(getLocation(), getSpeed(), course));
        if(state == MOVING_TO_POSITION && getLocation() == destination_coordinates) // the agent reached the destination
        {
            stop();
        }
    }
}

void Agent::stop() {
    state = STOPPED;
    destination = nullptr;
    destination_coordinates = getLocation();
    // course unchanged
}

void Agent::setCourse(double theta) {
    state = MOVING_ON_COURSE;
    course = theta;
    destination = nullptr;
    // destination_coordinates unchanged
}

double Agent::getCourse() const { return course; }

void Agent::setDestinationCoordinates(Point destination_) {
    state = MOVING_TO_POSITION;
    course = Point ::getAngle(getLocation(),destination_);
    destination = nullptr;
    destination_coordinates = destination_;
    // course unchanged
}

Point Agent::getDestinationCoordinates() const {
    return destination_coordinates;
}

void Agent::setDestination(std::shared_ptr<Structure> destination) {
    state = ON_DUTY;
    Agent::destination = destination;
    destination_coordinates = destination->getLocation();
    // course unchanged
}

std::shared_ptr<Structure> Agent::getDestination() const { return destination; }

// void Agent::setState(enum state state) { Agent::state = state; }

int Agent::getState() const { return state; }

void Agent::setSpeed(double speed) { Agent::speed = speed; }

double Agent::getSpeed() const { return speed; }

void Agent::setHealth(int health1) {
    if(health != 20)
        Agent::health = health1;
    if (health == 0)
        state = DEAD;
}

int Agent::getHealth() const { return health; }

void Agent::broadcast_current_state() const noexcept {
    Sim_object::broadcast_current_state();
    if (state == MOVING_TO_POSITION) {
        std::cout << " heading to: (" << getDestinationCoordinates().x << ", "
                  << getDestinationCoordinates().y << "), at speed: " << speed;
    } else if (state == DEAD) {
        std::cout << " dead";
    } else if (state == STOPPED) {
        std::cout << " stopped";
    }
}

void Agent::setState(enum Agent::state _state) {
    Agent::state = _state;
}
