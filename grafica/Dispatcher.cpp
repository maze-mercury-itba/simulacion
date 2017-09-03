#include "Dispatcher.h"
#include "../Intelligence/Intelligence.h"
#include "../World/World.h"
#include "../senact/senact.h"

enum dirs { W, A, S, D };
int parseAxis(bool pos, bool neg);


Dispatcher::Dispatcher(Graphic& g, rInfo_t * r) : g(g)
{
	this->r = r;
	activeRobot = 0;
	waitingNewPos = false;
	paused = false;
	simCounter = 0;
	simTimeout = DEFAULT_SIM_TIMEOUT;
	manual = false;
	memset(lastDrive, 0, sizeof(lastDrive));

	S_Init();
	W_setRobotConfiguration(&(r[0].r));
	initTodo(r[0].r);
	I_Init(manual ? MANUAL : AUTO1);
}

void Dispatcher::dispatch(Event * ev)
{
	switch (ev->name) {
	case RESIZE: {
		g.newDispSize(ev->x, ev->y);
		g.drawSimSpeed(float(simTimeout) / DEFAULT_SIM_TIMEOUT);
	} break;

	case SIMULATION_TIMEOUT: {
		if (!paused && !waitingNewPos) {
			if (++simCounter >= simTimeout) {
				simCounter = 0;
				W_Update();
				S_Update();	//en estas dos funcs habria que verificar el error
				I_Update();
			}
		}
	} break;

	case FRAME_TIMEOUT: {
		g.drawBackground();
		dpoint_t centerPoint;
		centerPoint.x = .15 / 2;
		centerPoint.y = .15 / 2;
		g.drawRobot(W_absolutePoint(centerPoint), W_getRobotPosition().angle);

		for (unsigned int i = 0; i < 2; i++) {
			g.drawSensorInfo(r[activeRobot].r.sensorArray[i], S_getStateValue(i));
		}
		g.showChanges();
	} break;

	case FAST_FORWARD: {
		if (--simTimeout <= MIN_SIM_TIMEOUT) {
			simTimeout = MIN_SIM_TIMEOUT;
			g.setButtonState(B_FF, false);
			g.drawButtons(B_FF);
		}
		else if (simTimeout == MAX_SIM_TIMEOUT - 1) {
			g.setButtonState(B_SLOW, true);
			g.drawButtons(B_SLOW);
		}
		g.drawSimSpeed(DEFAULT_SIM_TIMEOUT / float(simTimeout));
	} break;

	case SLOW_DOWN: {
		if (++simTimeout >= MAX_SIM_TIMEOUT) {
			simTimeout = MAX_SIM_TIMEOUT;
			g.setButtonState(B_SLOW, false);
			g.drawButtons(B_SLOW);
		}
		else if (simTimeout == MIN_SIM_TIMEOUT + 1) {
			g.setButtonState(B_FF, true);
			g.drawButtons(B_FF);
		}
		g.drawSimSpeed(DEFAULT_SIM_TIMEOUT / float(simTimeout));
	} break;

	case SWITCH_MODE: {
		manual = !manual;	//ESTO FALTA HACERLO!
		I_Init(manual ? MANUAL : AUTO1);
		memset(lastDrive, 0, sizeof(lastDrive));
	} break;

	case SWITCH_ROBOT: {
		activeRobot = !activeRobot;
		r[activeRobot].r.position = W_getRobotPosition().position;
		r[activeRobot].r.D_angle = W_getRobotPosition().angle;
		r[activeRobot].r.D_velocity = 0;
		r[activeRobot].r.R_velocity = 0;

		W_setRobotConfiguration(&(r[activeRobot].r));
		initTodo(r[activeRobot].r);

		g.newRobot(r[activeRobot].img, r[activeRobot].size);
	} break;

	case PAUSE: {
		if (paused == false) {
			paused = true;
			g.setButtonState(B_PAUSE, false);
			g.drawButtons(B_PAUSE);
		}
	} break;

	case PLAY: {
		if (paused == true) {
			paused = false;
			g.setButtonState(B_PAUSE, true);
			g.drawButtons(B_PAUSE);
		}
		else {
			g.drawSimSpeed(1.0);
			button_t b = (simTimeout == MIN_SIM_TIMEOUT) ? B_FF : (simTimeout == MAX_SIM_TIMEOUT ? B_SLOW : N_BUTTONS);
			if (b < N_BUTTONS) {
				g.setButtonState(b, true);
				g.drawButtons(b);
			}
			simTimeout = DEFAULT_SIM_TIMEOUT;
		}
	}
			   break;

	case NEWPOS: {
		if (waitingNewPos == true) {
			dpoint_t newPos = g.realFromPixel(ev->x, ev->y);
			if (newPos.x != DBL_MAX && newPos.y != DBL_MAX) {
				r[activeRobot].r.position = newPos;
				W_setRobotConfiguration(&(r[activeRobot].r));
				waitingNewPos = false;
				g.setButtonState(B_NEWPOS, true);
				g.drawButtons(B_NEWPOS);
			}
		}
		else {
			if (ev->x == -1) {
				waitingNewPos = true;
				g.setButtonState(B_NEWPOS, false);
				g.drawButtons(B_NEWPOS);
			}
		}
	} break;

	case EXIT: {; } break;

	case DESP: case DIR: case VEL: {
		if (manual && !paused && !waitingNewPos) {
			lastDrive[ev->name - VEL] = ev->x;	//vel esta primero de los tres en el enum
			I_Drive(lastDrive[DIR - VEL], lastDrive[DESP - VEL], lastDrive[0]);
		}
	} break;

	default: {; } break;
	}
}


int parseAxis(bool pos, bool neg) {
	return ((pos == true ? 1 : 0) - (neg == true ? 1 : 0));
}