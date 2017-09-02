#pragma once

#include <cstdint>
#include <allegro5\allegro.h>
#include <allegro5\timer.h>
#include <allegro5\events.h>
#include "Button.h"
#include <vector>
#include "Event.h"


#define	DEFAULT_SIM_TIMEOUT			10		//por default cada cuantos timeouts hago un update
#define MIN_SIM_TIMEOUT				5	
#define MAX_SIM_TIMEOUT				15

#if MAX_SIM_TIMEOUT < DEFAULT_SIM_TIMEOUT || MIN_SIM_TIMEOUT > DEFAULT_SIM_TIMEOUT
#error "los timeouts estan mal!! tiene que ser min <= default <= max"
#endif

//(va a ir cambiando con fastforward y eso), esta en segundos!
#ifdef DEGUB
#define	MIN_SIMULATION_TIMER		0.0001	//lo mas rapido que puede ir la simulacion
											// Cuanto mas chico es este valor, mas rapido va
#else
#define MIN_SIMULATION_TIMER		0.0002
#endif

#define FRAME_RATE		100	

class EventGenerator
{
public:
	EventGenerator(ALLEGRO_DISPLAY * display, const std::vector<Button> * b);
	~EventGenerator();
	Event * getNextEvent();
	bool isValid();

private:
	bool valid;

	ALLEGRO_TIMER * frameRateTimer;
	ALLEGRO_TIMER * simulationTimer;	
	ALLEGRO_EVENT_QUEUE * userEvs;
	ALLEGRO_EVENT_QUEUE * timers;
	const std::vector<Button> * b;
	bool joystick;
};