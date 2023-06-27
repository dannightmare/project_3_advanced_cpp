//
// Created by igor on 14/06/2023.
//

#ifndef PROJECT_3_AGENT_H
#define PROJECT_3_AGENT_H

#include "Model.h"

class Agent : public Sim_object {

    std::string name;
	Point destination;
	double speed;
    int state;
    int health;
    std::string type;

	protected:
	enum state {
		STOPPED,
		DEAD,
		MOVING_TO_POSITION
	};



	public:
    Agent(std::string & name_,Point& location,int speed_,int health);
	void move_to_place(Point& destination_);
	void move_to_direction(double theta);
	void update() override;
	void stop();
    std::string getType();
    std::string getName();

};


#endif //PROJECT_3_AGENT_H
