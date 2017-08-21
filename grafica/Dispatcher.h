#pragma once
#include "EventGenerator.h"
#include "Graphic.h"

class Dispatcher {
public:
	Dispatcher(Graphic& g, robot_t& s);
	void dispatch(Event * ev);

private:
	bool waitingNewPos;
	bool paused;
	bool manual;
	uint16_t simTimeout;
	uint16_t simCounter;
	Graphic& g;
	robot_t& r;
};

