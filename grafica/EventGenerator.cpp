#include "EventGenerator.h"
#include <cstdio>
#include <iterator>
#include <vector>

template <typename T> int sign(T val) {
	return (T(0) < val) - (val < T(0));
}

bool isFromAxis(const ALLEGRO_EVENT ev, int a);
Event * getDriveEvent(const ALLEGRO_EVENT ev, bool joystick);


EventGenerator::EventGenerator(ALLEGRO_DISPLAY * display, const std::vector<Button> * b)
{
	userEvs = timers = NULL;
	simulationTimer = NULL;
	frameRateTimer = NULL;
	joystick = false;
	valid = false;

	if (b != NULL) {
		if (al_install_keyboard()) {
			if (al_install_mouse()) {
				if ((userEvs = al_create_event_queue()) != NULL && (timers = al_create_event_queue()) != NULL ) {
					if ((frameRateTimer = al_create_timer(1.0 / (double)FRAME_RATE)) != NULL) {
						if ((simulationTimer = al_create_timer(MIN_SIMULATION_TIMER)) != NULL) {
							
							al_register_event_source(userEvs, al_get_keyboard_event_source());
							al_register_event_source(timers, al_get_timer_event_source(frameRateTimer));
							al_register_event_source(timers, al_get_timer_event_source(simulationTimer));
							al_register_event_source(userEvs, al_get_mouse_event_source());
							
							if (display != NULL) {
								al_register_event_source(userEvs, al_get_display_event_source(display));
							}

							if (al_install_joystick()) {
								al_register_event_source(userEvs, al_get_joystick_event_source());

								if (al_get_num_joysticks() > 0) {
									joystick = true;
								}
							}
							valid = true;
							this->b = b;
							al_start_timer(frameRateTimer);
							al_start_timer(simulationTimer);
						}
						else {
							fprintf(stderr, "Unable to create simulation timer\n");
							al_destroy_timer(frameRateTimer);
							frameRateTimer = NULL;
							al_destroy_event_queue(userEvs); al_destroy_event_queue(timers);
							userEvs = NULL;	timers = NULL;
						}
					}
					else {
						fprintf(stderr, "Unable to create framerate timer\n");
						al_destroy_event_queue(userEvs); al_destroy_event_queue(timers);
						userEvs = NULL;	timers = NULL;
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
	if (valid) {
		al_destroy_timer(simulationTimer);
		simulationTimer = NULL;
		al_destroy_timer(frameRateTimer);
		frameRateTimer = NULL;
		al_destroy_event_queue(userEvs); al_destroy_event_queue(timers);
		userEvs = NULL;	timers = NULL;
		al_uninstall_keyboard();
	}
}

Event * EventGenerator::getNextEvent()
{
	Event * e = nullptr;
	ALLEGRO_EVENT ev;
	if (valid) {
		if (al_get_next_event(userEvs, &ev) || al_get_next_event(timers, &ev)) {
			switch (ev.type) {
			case ALLEGRO_EVENT_TIMER: {
				if (ev.timer.source == frameRateTimer)
					e = new Event(FRAME_TIMEOUT);
				else if (ev.timer.source == simulationTimer)
					e = new Event(SIMULATION_TIMEOUT);
			} break;

			case ALLEGRO_EVENT_KEY_UP: case ALLEGRO_EVENT_KEY_DOWN: { //primero descarto las keys que no son para manejar
				if (ev.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
					e = new Event(EXIT);
					break;
				}
			}	//ACA NO HAY BREAK A PROPOSITO!! hago lo mismo para teclas que no son las de arriba que para joystick

			case ALLEGRO_EVENT_JOYSTICK_AXIS: {
				e = getDriveEvent(ev, joystick);
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
					case B_NEWPOS:	e = new Event(NEWPOS, -1);		break;
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

			case ALLEGRO_EVENT_JOYSTICK_CONFIGURATION: {
				al_reconfigure_joysticks();
				joystick = al_get_num_joysticks() > 0 ? true : false;
			} break;
			default: {
				e = new Event(NO_EVENT);
			} break;
			}
		}

		if (e == nullptr) {
			e = new Event(NO_EVENT);
		}
	}

	return e;
}

bool EventGenerator::isValid()
{
	return valid;
}


enum axis { PAD_X, PAD_Y, R_Y, N_AXIS };


//POSITIVO ABAJO Y A LA DERECHA, EN EL CENTRO 0

Event * getDriveEvent(const ALLEGRO_EVENT ev, bool joystick)
{
	Event * newEv = nullptr;

	if (joystick)
	{
		if (ev.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
			if (isFromAxis(ev, PAD_X)) {
				newEv = new Event(DIR, sign(ev.joystick.pos));	//POSITIVO A LA DERECHA
			}
			else if (isFromAxis(ev, PAD_Y)) {
				newEv = new Event(DESP, -sign(ev.joystick.pos)); //paso positivo para arriba
			}
			else if (isFromAxis(ev, R_Y)) {
				newEv = new Event(VEL, int((-127.0) * ev.joystick.pos));
			}
		}
	}
	else if ( ev.type == ALLEGRO_EVENT_KEY_UP || ev.type == ALLEGRO_EVENT_KEY_DOWN ) {
		ALLEGRO_KEYBOARD_STATE kb;
		al_get_keyboard_state(&kb);

		switch (ev.keyboard.keycode) {

		case ALLEGRO_KEY_W: case ALLEGRO_KEY_S: {	    //DESP
			newEv = new Event(DESP, al_key_down(&kb, ALLEGRO_KEY_W) - al_key_down(&kb, ALLEGRO_KEY_S));
		} break;

		case ALLEGRO_KEY_A: case ALLEGRO_KEY_D: {	    //DIR
			newEv = new Event(DIR, al_key_down(&kb, ALLEGRO_KEY_D) - al_key_down(&kb, ALLEGRO_KEY_A));
		} break;

		case ALLEGRO_KEY_UP: case ALLEGRO_KEY_DOWN: {	//VEL
			newEv = new Event(VEL, 127 * (al_key_down(&kb, ALLEGRO_KEY_UP) - al_key_down(&kb, ALLEGRO_KEY_DOWN)));
		} break;
		}
	}

	return newEv;
}



#define PAD_STICK_X	2
#define PAD_AXIS_X	0
#define PAD_STICK_Y	2
#define PAD_AXIS_Y	1

#define R_STICK_X	0	//X NO SE USA 
#define R_AXIS_X	2
#define R_STICK_Y	1
#define R_AXIS_Y	0

static int jaxis [N_AXIS][2] = { {PAD_STICK_X, PAD_AXIS_X}, {PAD_STICK_Y, PAD_AXIS_Y},
									{R_STICK_Y, R_AXIS_Y} };

bool isFromAxis(const ALLEGRO_EVENT ev, int a)
{
	return (a < N_AXIS && ev.joystick.stick == jaxis[a][0] && ev.joystick.axis == jaxis[a][1]);
}

