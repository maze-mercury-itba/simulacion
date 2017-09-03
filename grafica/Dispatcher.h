#pragma once
#include "EventGenerator.h"
#include "Graphic.h"

typedef struct {
	robot_t r;
	const char * img;
	dpoint_t size;
} rInfo_t;


class Dispatcher {
public:
	Dispatcher(Graphic& g, rInfo_t * r);
	void dispatch(Event * ev);

private:
	bool waitingNewPos;
	bool paused;
	bool manual;
	bool activeRobot;
	int lastDrive [3];	//vel, desp, dir, en el orden que estan en el enum

	uint16_t simTimeout;
	uint16_t simCounter;
	Graphic& g;
	rInfo_t * r;
};

