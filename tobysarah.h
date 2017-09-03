#pragma once
#include "World\WorldStructures.h"

#define N_POINTS	100

#define L_SARAH		0.05	// lado EN METROS
#define R_TOBY		0.075	// radio en metros

#define S_IMG		"grafica/s.png"
#define T_IMG		"grafica/t.png"

robot_t loadToby(dpoint_t p0);
dpoint_t tobySize();

robot_t loadSarah(dpoint_t p0);
dpoint_t sarahSize();
