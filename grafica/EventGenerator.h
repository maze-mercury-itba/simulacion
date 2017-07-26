#pragma once

#include <cstdint>
#include <allegro5\allegro.h>
#include <allegro5\timer.h>
#include <allegro5\events.h>

enum events { NO_EVENT, UP_UP, UP_DOWN, LEFT_UP, LEFT_DOWN,
	RIGHT_UP, RIGHT_DOWN, DOWN_UP, DOWN_DOWN,A_UP,A_DOWN,D_UP,D_DOWN,
	FAST_FORWARD, SLOW_DOWN, FRAME_TIMEOUT, SIMULATION_TIMEOUT, EXIT };

#define FRAME_RATE					60	//en FPS


#define	INITIAL_SIMULATION_TIMER	0.03		//cada cuanto hago update world inicialmente
//(va a ir cambiando con fastforward y eso), esta en segundos!
#define	MIN_SIMULATION_TIMER		0.01	//lo mas rapido que puede ir la simulacion
#define STEP_SIMULATION_TIMER		0.01	//de a cuanto se va cambiando la velocidad
#define MAX_SIMULATION_TIMER		1		//lo mas lento

class EventGenerator
{
public:
	EventGenerator(ALLEGRO_DISPLAY * display);
	~EventGenerator();
	uint16_t getNextEvent();


private:
	ALLEGRO_TIMER * frameRateTimer;
	ALLEGRO_TIMER * simulationTimer;	//problema: el handler queda adentro de getNextEvent?
	ALLEGRO_EVENT_QUEUE * evQ;

};