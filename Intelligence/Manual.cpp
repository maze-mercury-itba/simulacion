//
//#include "Manual.h"
//
////
//// x puede tomar valores de -1, 0, 1.
//// y puede tomar valores de -1, 0, 1.
//// vel toma valores enteros entre........... y ...........
//void startManual(int x, int y, int vel)
//{
//	if (x == -1)
//	{
//		if (y == -1)
//		{
//			backLeft(); //en diagonal hacia atras e izq
//		}
//
//		else if (y == 0)
//		{
//			turnLeft(); //hacia izq
//		}
//
//		else if (y == 1)
//		{
//			forwardLeft(); //hacia adelante e izq
//		}
//
//		else
//		{
//			//INDICAR ERROR?
//		}
//	}
//	else if (x == 0)
//	{
//		if (y == -1)
//		{
//			//hacia atras
//			goBack();
//		}
//
//		else if (y == 0)
//		{
//			//QUIETO
//		}
//
//		else if (y == 1)
//		{
//			moveForward();
//		}
//
//		else
//		{
//			//INDICAR ERROR?
//		}
//	}
//	else if (x == 1)
//	{
//		if (y == -1)
//		{
//			backRight(); //hacia atras y derecha
//		}
//
//		else if (y == 0)
//		{
//			//hacia derecha
//			turnRight();
//		}
//
//		else if (y == 1)
//		{
//			forwardRight(); //hacia adelante y derecha
//		}
//
//		else
//		{
//			//INDICAR ERROR?
//		}
//	}
//	else;
//		//indicar error??
//}

#include "Intelligence.h"
#include "../senact/senact.h"
#include <cmath>
#include "Manual.h"
#include <algorithm>

using namespace std;
#define MOT_DUTY_DESPLAZAMIENTO_NOM	200
#define MOT_DUTY_ROTACION_NOM		180
#define MIN_VEL						(-128)
#define MAX_VEL						127

#define MAX_MOT	(MOT_DUTY_DESPLAZAMIENTO_NOM + MOT_DUTY_ROTACION_NOM + 2*(max(abs(MAX_VEL), abs(MIN_VEL))>>1))

//#define MAX_IZQ (MOT_DUTY_DESPLAZAMIENTO_NOM + MOT)

static int dutyMotIzq = 0, dutyMotDer = 0;


void I_Drive(int direccion, int desplazamiento, int velocidad) {
	int desp, dir;
	desp = desplazamiento * (MOT_DUTY_DESPLAZAMIENTO_NOM + (velocidad >> 1));
	dir = direccion * (MOT_DUTY_ROTACION_NOM + (velocidad >> 1));
	dutyMotIzq = desp + dir;
	dutyMotDer = desp - dir;

	dutyMotIzq = (int)round(dutyMotIzq * 100 / ((float)MAX_MOT));	//paso a porcentaje
	dutyMotDer = (int)round(dutyMotDer * 100 / ((float)MAX_MOT));

	startManual();
}

void startManual(void)
{
	S_setActuatorMov(MOTOR_L, dutyMotIzq);
	S_setActuatorMov(MOTOR_R, dutyMotDer);
}
