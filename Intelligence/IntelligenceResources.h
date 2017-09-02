#ifndef INTELLIGENCERESOURCES_H
#define INTELLIGENCERESOURCES_H

#define SENSORS_AMMOUNT		3
//#define ANGLE_SECOND_SENSOR	0.6981317
//#define DISTANCE_PJ	
#define MIN_DISTANCE		20

#define RIGHT_MOTOR			0
#define LEFT_MOTOR			1

#define ANGLE_RANGES		12			//cantidad de rangos de angulos, de a 30 grados (ver de usar arreglo de estos ranges en vez de la estructura del .h)
#define ROBOT_WIDTH			15			//ver en cm o en que medida
#define ROBOT_LENGTH		15
#define LEFT_REFERENCE		(ROBOT_LENGTH/2)
#define FRONT_REFERENCE		(ROBOT_WIDTH/2)
#define PI					3.14159265358979323846

typedef struct
{
	int angle;
	int measuredDistance;
	int fromX;
	int fromY;
}sensorInfo_s;

#endif //INTELLIGENCERESOURCES_H