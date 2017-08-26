#pragma once
#include <cstdint>

typedef enum events {
	NO_EVENT, UP_UP, UP_DOWN, LEFT_UP, LEFT_DOWN,
	RIGHT_UP, RIGHT_DOWN, DOWN_UP, DOWN_DOWN, A_UP, A_DOWN, D_UP, D_DOWN, NEWPOS, SWITCH_MODE,
	FAST_FORWARD, SLOW_DOWN, FRAME_TIMEOUT, SIMULATION_TIMEOUT, PLAY, PAUSE, SWITCH_ROBOT, RESIZE, EXIT
} event_t;

class Event {
public:
	Event(event_t name, uint16_t x = 0, uint16_t y = 0) : name(name),x(x), y(y) { ; };
	const event_t name;
	const uint16_t x, y;
};