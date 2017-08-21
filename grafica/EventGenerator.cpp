#include "EventGenerator.h"
#include <cstdio>
#include <iterator>
#include <vector>


EventGenerator::EventGenerator(ALLEGRO_DISPLAY * display, const std::vector<Button> * b)
{
	evQ = NULL;
	simulationTimer = NULL;
	frameRateTimer = NULL;

	if (b != NULL) {
		if (al_install_keyboard()) {
			if (al_install_mouse()) {
				if ((evQ = al_create_event_queue()) != NULL) {
					if ((frameRateTimer = al_create_timer(1.0 / (double)FRAME_RATE)) != NULL) {
						if ((simulationTimer = al_create_timer(MIN_SIMULATION_TIMER)) != NULL) {
							al_register_event_source(evQ, al_get_keyboard_event_source());
							al_register_event_source(evQ, al_get_timer_event_source(frameRateTimer));
							al_register_event_source(evQ, al_get_timer_event_source(simulationTimer));
							al_register_event_source(evQ, al_get_mouse_event_source());
							if (display != NULL)
								al_register_event_source(evQ, al_get_display_event_source(display));

							this->b = b;
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
				fprintf(stderr, "Unable to install mouse\n");
			}
		}
		else {
			fprintf(stderr, "Unable to install keyboard\n");
		}
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

Event * EventGenerator::getNextEvent()
{
	Event * e = nullptr;
	ALLEGRO_EVENT ev;

	if (al_get_next_event(evQ, &ev)) {
		switch (ev.type) {
		case ALLEGRO_EVENT_TIMER: {
			if (ev.timer.source == frameRateTimer)
				e = new Event(FRAME_TIMEOUT);
			else if (ev.timer.source == simulationTimer)
				e = new Event(SIMULATION_TIMEOUT);
		} break;

		case ALLEGRO_EVENT_KEY_UP: case ALLEGRO_EVENT_KEY_DOWN: {
			switch (ev.keyboard.keycode) {
			case ALLEGRO_KEY_UP: {
				e = new Event(ev.type == ALLEGRO_EVENT_KEY_UP ? UP_UP : UP_DOWN);
			} break;

			case ALLEGRO_KEY_DOWN: {
				e = new Event(ev.type == ALLEGRO_EVENT_KEY_UP ? DOWN_UP : DOWN_DOWN);
			} break;

			case ALLEGRO_KEY_LEFT: {
				e = new Event(ev.type == ALLEGRO_EVENT_KEY_UP ? LEFT_UP : LEFT_DOWN);
			} break;

			case ALLEGRO_KEY_RIGHT: {
				e = new Event(ev.type == ALLEGRO_EVENT_KEY_UP ? RIGHT_UP : RIGHT_DOWN);
			} break;

			case ALLEGRO_KEY_A: {
				e = new Event(ev.type == ALLEGRO_EVENT_KEY_UP ? A_UP : A_DOWN);
			} break;

			case ALLEGRO_KEY_D: {
				e = new Event(ev.type == ALLEGRO_EVENT_KEY_UP ? D_UP : D_DOWN);
			} break;

			case ALLEGRO_KEY_ESCAPE: {
				e = new Event(EXIT);
			} break;
			}
		} break;

		case ALLEGRO_EVENT_DISPLAY_CLOSE: {
			e = new Event(EXIT);
		} break;

		case ALLEGRO_EVENT_DISPLAY_RESIZE: {
			e = new Event(RESIZE, ev.display.width, ev.display.height);
		} break;

		case ALLEGRO_EVENT_MOUSE_BUTTON_UP: {
			std::vector<Button>::const_iterator it = b->begin();
			while (it != b->end() && !it->wasPressed(ev.mouse.x, ev.mouse.y)) {
				it++;
			} 

			if (it != b->end()) {
				switch (it->getName()) {
				case B_FF:		e = new Event(FAST_FORWARD);	break;
				case B_SLOW:	e = new Event(SLOW_DOWN);		break;
				case B_NEWPOS:	e = new Event(NEWPOS, UINT16_MAX, UINT16_MAX);		break;
				case B_PAUSE:	e = new Event(PAUSE);			break;
				case B_PLAY:	e = new Event(PLAY);			break;
				case B_SWITCH:	e = new Event(SWITCH_ROBOT);	break;
				case B_MODE:	e = new Event(SWITCH_MODE);	break;
				}
			}
			else {
				e = new Event(NEWPOS, ev.mouse.x, ev.mouse.y);
			}
		} break;
		
		default: {
			e = new Event(NO_EVENT);
		} break;
		}
	}
	else {
		e = new Event(NO_EVENT);
	}

	return e;
}