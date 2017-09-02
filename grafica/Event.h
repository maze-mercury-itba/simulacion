#pragma once
#include <cstdint>

typedef enum events {
	NO_EVENT, VEL, DESP, DIR,
	NEWPOS, SWITCH_MODE, FAST_FORWARD, SLOW_DOWN, FRAME_TIMEOUT, SIMULATION_TIMEOUT, PLAY, PAUSE,
	SWITCH_ROBOT, RESIZE, EXIT
} event_t;

class Event {
public:
	Event(event_t name, uint16_t x = 0, uint16_t y = 0) : name(name),x(x), y(y) { ; };
	const event_t name;
	const int16_t x, y;
};