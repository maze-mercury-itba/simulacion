#include <cstdio>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include "../World/World.h"

#define PI 3.14159265359
#define MAX_VEL_R (PI/100)
#define MAX_VEL_D 1
#define ESCALA	(.15/50)

typedef struct{
	double right;
	double left;
}actuator_percentage_t;

static actuator_percentage_t percentage_old, percentage_new;

static void init_random (void){		//inicializacion para generar numeros random
	time_t t;
	srand((unsigned) time(&t));
}

static uint16_t generate_random (uint16_t max){		//generador de random (>0) a partir del valor max
	return (rand()%max);
}

static uint16_t convert_data (double data_r, double data_l, double * velocidad_d, double * velocidad_r){
	if((data_l<=100) && (data_l>=-100) && (data_r<=100) && (data_r>=-100)) //probamos que no se encuentre dentro del rango
	{
		*velocidad_d = (((data_l + data_r) / 200)*MAX_VEL_D);   //Calculo de velocidades
		*velocidad_r = (((data_l - data_r) / 200)*MAX_VEL_R);	
		return 1;
	}
	else
		return 0;
}

static void magic (void){		// Handler de errores y convercion
	//no magic example
	double dvelocity, rvelocity;
	double *p_dvelocity = &dvelocity, *p_rvelocity = &rvelocity;
	if(convert_data (percentage_new.right, percentage_new.left, p_dvelocity, p_rvelocity)){
		percentage_old.right = percentage_new.right;
		percentage_old.left = percentage_new.left;
		W_configureRobot (dvelocity * ESCALA, rvelocity);
	}


}

void S_setActuatorMov(uint16_t actuator_id, int16_t actuator_percentage){	//guardado de info
	switch(actuator_id)
	{
		case 0:
		percentage_new.left = actuator_percentage;
		break;

		case 1:
		percentage_new.right = actuator_percentage;
		break;
	}
}

uint16_t S_Update (void){
		magic();
		return 0;
}

void S_Init (void){
	init_random();
	percentage_old.right = 0;
	percentage_old.left = 0;

}
