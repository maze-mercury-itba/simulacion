#pragma once

#include <cstdint>
#include <allegro5\allegro.h>
#include <allegro5\timer.h>
#include <allegro5\events.h>
#include "Button.h"
#include <vector>
#include "Event.h"


#define FRAME_RATE					60	//en FPS


#define	DEFAULT_SIM_TIMEOUT			10		//por default cada cuantos timeouts hago un update
#define MIN_SIM_TIMEOUT				5	
#define MAX_SIM_TIMEOUT				15

#if MAX_SIM_TIMEOUT < DEFAULT_SIM_TIMEOUT || MIN_SIM_TIMEOUT > DEFAULT_SIM_TIMEOUT
#error "los timeouts estan mal!! tiene que ser min <= default <= max"
#endif

//(va a ir cambiando con fastforward y eso), esta en segundos!
#define	MIN_SIMULATION_TIMER		0.0005	//lo mas rapido que puede ir la simulacion

class EventGenerator
{
public:
	EventGenerator(ALLEGRO_DISPLAY * display, const std::vector<Button> * b);
	~EventGenerator();
	Event * getNextEvent();

private:
	ALLEGRO_TIMER * frameRateTimer;
	ALLEGRO_TIMER * simulationTimer;	
	ALLEGRO_EVENT_QUEUE * evQ;
	const std::vector<Button> * b;
};