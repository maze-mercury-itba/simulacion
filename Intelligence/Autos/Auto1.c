
#include "Auto1.h"
#include <stdbool.h>

#define I_MIN_DISTANCE_BETWEEN_POINTS	0.5
#define I_MIN_DISTANCE_AHEAD		5

static int PointsOnXAxis[2]; //GUARDO ITERADORES!!


void startAuto1(const dpoint_t * allCoordinates)
{
	int i;
	checkPointsOnXAxis(allCoordinates);
	if (PointsOnXAxis[0] != (SENSORS_AMMOUNT + 1))
	{
		if (PointsOnXAxis[1] != (SENSORS_AMMOUNT + 1))
		{
			if (!somethingAhead(allCoordinates))
			{
				double inclination = getInclination(allCoordinates[PointsOnXAxis[0]], allCoordinates[PointsOnXAxis[1]]);
				actOverInclination(inclination);
			}
			else
				trunRight();
		}
		else
		{
			if (!somethingAhead(allCoordinates))
				moveFoward();
			else
				turnRight();
		}
	}
	return;
}


//////////////////////////////////////////////////////////////////////
/*Funcion que no recibe nada y chequea el arreglo de allCoordinates buscando los numeros negativos de x
y encontrando 2 puntos guarda el iterador de allCoordinates en PointsOnXAxis, ordenandolos de menor "y" de
coordinate_s a mayor "y" de coordinate_s
*/
//////////////////////////////////////////////////////////////////////
static void checkPointsOnXAxis(const dpoint_t * allCoordinates)
{
	int i;
	for (i = 0; i<2; i++)//Inicializo el arreglo de 2 ints en SENSOR AMOUNT + 1 ya que el arreglo en ese iterador no existe
		PointsOnXAxis[i] = SENSORS_AMMOUNT + 1;
	for (i = 0; (i<SENSORS_AMMOUNT) && (allCoordinates[i].x >= 0); i++);//Me muevo hasta encontrar un punto en x negativo o hasta recorrer todos los puntos obtenidos
	if (allCoordinates[i].x<0)	//Chequeo si salio del for porque un sensor encontro un pto negativo
	{
		PointsOnXAxis[0] = i;		//Guardo el numero del pto en el erreglo PointsOnXAxis
		bool exit = false;
		for (i++; (i<SENSORS_AMMOUNT) && (exit == false); i++)
		{
			if (allCoordinates[i].x <0)
			{
				if ((allCoordinates[i].y)>(allCoordinates[PointsOnXAxis[0]].y + I_MIN_DISTANCE_BETWEEN_POINTS))
				{
					PointsOnXAxis[1] = i;
					exit = true;
				}
				else if ((allCoordinates[i].y)<(allCoordinates[PointsOnXAxis[0]].y - I_MIN_DISTANCE_BETWEEN_POINTS))
				{
					PointsOnXAxis[1] = PointsOnXAxis[0];
					PointsOnXAxis[0] = i;
					exit = true;
				}
			}
		}
	}
	return;
}

//////////////////////////////////////////////////////////////////////
/*Funcion que devuelve la pendiente que se forma entre 2 puntos
coordinate_s
*/
//////////////////////////////////////////////////////////////////////
static double getInclination(dpoint_t p0, dpoint_t p1)
{
	double deltaX = p0.x - p1.x;
	double deltaY = p0.y - p1.y;
	return (deltaX / deltaY);
}

//////////////////////////////////////////////////////////////////////
/*Funcion que devuelve true si hay algo delante del robot
o false si no hay nada delante del robot*/
//////////////////////////////////////////////////////////////////////
static bool somethingAhead(const dpoint_t * allCoordinates)
{
	int i;
	bool isThereSomethingAhead = false;
	for (i = 0;(i < SENSORS_AMMOUNT) && (isThereSomethingAhead == false); i++)
		if ((allCoordinates->y <= 0) && (allCoordinates->y >= -I_MIN_DISTANCE_AHEAD) && (allCoordinates->x >= -I_MIN_DISTANCE_AHEAD)
			&& (allCoordinates->x <= I_MIN_DISTANCE_AHEAD))
			isThereSomethingAhead = true;
	return isThereSomethingAhead;
}

static void turnRight(void)
{
	//Hacer esta funcion!!!!!
}