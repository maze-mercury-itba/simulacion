#include "EventGenerator.h"
#include <cstdio>


EventGenerator::EventGenerator(ALLEGRO_DISPLAY * display)
{
	evQ = NULL;
	simulationTimer = NULL;
	frameRateTimer = NULL;

		if (al_install_keyboard()) {
		if ((evQ = al_create_event_queue()) != NULL) {
			if ((frameRateTimer = al_create_timer(1.0 / (double)FRAME_RATE)) != NULL) {
				if ((simulationTimer = al_create_timer(INITIAL_SIMULATION_TIMER)) != NULL)	{
					al_register_event_source(evQ, al_get_keyboard_event_source());
					al_register_event_source(evQ, al_get_timer_event_source(frameRateTimer));
					al_register_event_source(evQ, al_get_timer_event_source(simulationTimer));

					if (display != NULL)
						al_register_event_source(evQ, al_get_display_event_source(display));

					al_start_timer(frameRateTimer);
					al_start_timer(simulationTimer);
				}
				else {
					fprintf(stderr, "Unable to create simulation timer\n");
					al_destroy_timer(frameRateTimer);
					frameRateTimer = NULL;
					al_destroy_event_queue(evQ);
					evQ = NULL;
				}
			}
			else {
				fprintf(stderr, "Unable to create framerate timer\n");
				al_destroy_event_queue(evQ);	
				evQ = NULL;
			}
		}
		else {
			fprintf(stderr, "Unable to create event queue\n");
			al_uninstall_keyboard();
		}
	}
	else {
		fprintf(stderr, "Unable to install keyboard\n");
	}
}


EventGenerator::~EventGenerator()
{
	al_destroy_timer(simulationTimer);
	simulationTimer = NULL;
	al_destroy_timer(frameRateTimer);
	frameRateTimer = NULL;
	al_destroy_event_queue(evQ);
	evQ = NULL;
	al_uninstall_keyboard();
}

uint16_t EventGenerator::getNextEvent()
{
	uint16_t evCode = NO_EVENT;
	ALLEGRO_EVENT ev;

	if (al_get_next_event(evQ, &ev)) {  //faltan los eventos de display!!
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			if (ev.timer.source == frameRateTimer)
				evCode = FRAME_TIMEOUT;
			else if (ev.timer.source == simulationTimer)
				evCode = SIMULATION_TIMEOUT;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_UP || ev.type == ALLEGRO_EVENT_KEY_DOWN )
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP:
				evCode = (ev.type == ALLEGRO_EVENT_KEY_UP ? UP_UP : UP_DOWN);
				break;

			case ALLEGRO_KEY_DOWN:
				evCode = (ev.type == ALLEGRO_EVENT_KEY_UP ? DOWN_UP : DOWN_DOWN);
				break;

			case ALLEGRO_KEY_LEFT:
				evCode = (ev.type == ALLEGRO_EVENT_KEY_UP ? LEFT_UP : LEFT_DOWN);
				break;

			case ALLEGRO_KEY_RIGHT:
				evCode = (ev.type == ALLEGRO_EVENT_KEY_UP ? RIGHT_UP :RIGHT_DOWN);
				break;

				// ESTOS HANDLERS DE EVENTOS NO VAN A QUEDAR ACA!!
			case ALLEGRO_KEY_F:
				if (ev.type == ALLEGRO_EVENT_KEY_UP && al_get_timer_speed(simulationTimer) > MIN_SIMULATION_TIMER)
					al_set_timer_speed(simulationTimer, al_get_timer_speed(simulationTimer) - STEP_SIMULATION_TIMER);
				break;

			case ALLEGRO_KEY_S:
				if (ev.type == ALLEGRO_EVENT_KEY_UP && al_get_timer_speed(simulationTimer) < MAX_SIMULATION_TIMER)
					al_set_timer_speed(simulationTimer, al_get_timer_speed(simulationTimer) + STEP_SIMULATION_TIMER);
				break;

			case ALLEGRO_KEY_ESCAPE:
				evCode = EXIT;
				break;
			}

		//FALTAN EVENTOS DE DISPLAY
	}

	return evCode;
}


