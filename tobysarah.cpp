#include "tobysarah.h"

#include "allegro5/allegro.h"
#include "allegro5/allegro_primitives.h"
#include <cmath>
#include <algorithm>
#include "vector.h"
#include "World\WorldStructures.h"

void pointsOnPolygon(double l, unsigned int n, unsigned int sides, double p0x, double p0y, dpoint_t * p);
void pointsOnCircle(double r, double cx, double cy, unsigned int n, double angle0, dpoint_t * p);	//n puntos en un circulo de radio R alrededor de cx, cy


using namespace std;

#define PI	3.14159




robot_t loadToby(dpoint_t p0)
{
	robot_t r;
	r.D_angle = 0;	r.D_velocity = 0; r.position = p0;
	r.R_point.x = r.R_point.y = R_TOBY;
	r.amountOfPoints = N_POINTS;
	r.robotPoints = new dpoint_t[N_POINTS];

	pointsOnCircle(R_TOBY, R_TOBY, R_TOBY, N_POINTS, 0, r.robotPoints);
	
	r.sensorArray[0].angle = 0;
	r.sensorArray[1].angle = 7 * PI / 4;

	for (unsigned int i = 0; i <= 1; i++) { //para cada sensor de toby!
		r.sensorArray[i].positionOnRobot.x = R_TOBY * (1 + sin(r.sensorArray[i].angle));
		r.sensorArray[i].positionOnRobot.y = R_TOBY * (1 - cos(r.sensorArray[i].angle));
	}

	return r;
}

dpoint_t tobySize()
{
	dpoint_t s = { 2 * R_TOBY, 2 * R_TOBY };
	return s;
}


robot_t loadSarah(dpoint_t p0)
{
	robot_t r;
	r.D_angle = 0;	r.D_velocity = 0; r.position = p0;
	r.R_point.x = r.R_point.y = L_SARAH * (1.0+sqrt(2))/2.0; //el medio de un octogono regular de radio L_SARAH
	r.amountOfPoints = N_POINTS;
	r.robotPoints = new dpoint_t[N_POINTS];

	pointsOnPolygon(L_SARAH, N_POINTS, 8, 0, 0, r.robotPoints);

	r.sensorArray[0].angle = PI / 4;
	r.sensorArray[1].angle = 7 * PI / 4;

	float d = L_SARAH / (2.0 * sqrt(2)); //los sensores estan a la mitad de los segmentos que estan en diagonal
	r.sensorArray[0].positionOnRobot.y = r.sensorArray[1].positionOnRobot.y = d;
	r.sensorArray[0].positionOnRobot.x = 3 * d + L_SARAH; //las dos mitades de segmento diagonal de la izq, el lado y el segm de la der
	r.sensorArray[1].positionOnRobot.x = d;

	return r;
}

dpoint_t sarahSize()
{
	dpoint_t s;
	s.x = L_SARAH * (1 + sqrt(2));
	s.y = s.x;
	return s;
}


void pointsOnCircle(double r, double cx, double cy, unsigned int n, double angle0, dpoint_t * p)
{
	if (p == nullptr || r <= 0 || n == 0)
		return;

	double t;

	for (unsigned int i = 0; i < n; i++) {
		t = double(i + 1) * 2.0 * PI / double(n) + angle0;
		p[i].x = r * cos(t) + cx;
		p[i].y = r * sin(t) + cy;
	}
}
void pointsOnPolygon(double l, unsigned int n, unsigned int sides, double p0x, double p0y, dpoint_t * p)
{
	if (p == nullptr || n < sides || sides < 3 || l <= 0)
		return;

	double angle0 = PI / sides;
	dpoint_t * vertex = new dpoint_t[sides];
	double r = l / (2.0 * sin(angle0));

	pointsOnCircle(r, p0x + r, p0y + r, sides, PI / 2 - angle0, vertex);	//arriba queda una linea recta siempre (:

	unsigned int nPerSide = (n - sides) / sides + ((n - sides) % sides == 0 ? 0 : 1);
	//si nRemaining%sides != 0, en algun/os lado van a quedar menos 

	//pongo puntos en las rectas que unen los vertices: primero de (0,1) a (sides-2, sides-1)
	double t;
	unsigned int start = 0, end = 0; //el primer segmento siempre tiene nPerSide puntos
	dpoint_t offset = { r*1.1, r*1.1 };	 //aca voy a ir guardando que tan corrido me quedo (nunca va a ser mas que r)

	for (unsigned int i = 0; i < sides - 1; i++) {
		end += nPerSide + 1;
		if (i + 1 >((n - sides) % sides)) {
			end--;
		}

		for (unsigned int j = start; j < end; j++) {
			t = double(j - start) / double(end - start);		// para poner n puntos, divido el segmento en n

			p[j].x = vertex[i].x + t *(vertex[i + 1].x - vertex[i].x);
			p[j].y = vertex[i].y + t *(vertex[i + 1].y - vertex[i].y);

			offset.x = std::min(offset.x, p[j].x - p0x);
			offset.y = std::min(offset.y, p[j].y - p0y);
		}

		start = end;
	}

	//queda hacer los puntos de (sides-1, 0) (start quedo en end del loop de antes)
	for (unsigned int j = start; j < n; j++) {
		t = double(j - start) / double(n - start);		// nPerSide dividen el segmento en nPerSide+1 partes

		p[j].x = vertex[sides - 1].x + t *(vertex[0].x - vertex[sides - 1].x);
		p[j].y = vertex[sides - 1].y + t *(vertex[0].y - vertex[sides - 1].y);

		offset.x = std::min(offset.x, p[j].x - p0x);
		offset.y = std::min(offset.y, p[j].y - p0y);
	}

	for (unsigned int i = 0; i < n; i++) {
		p[i].x -= offset.x;
		p[i].y -= offset.y;
	}


	delete[] vertex;
}
