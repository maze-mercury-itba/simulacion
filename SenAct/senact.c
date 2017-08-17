#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef struct{
	double right;
	double left;
}actuator_percentage_t;

static actuator_percentage_t percentage_old, percentage_new;

static void init_random (void){
	time_t t;
	srand((unsigned) time(&t));
}

static uint16_t generate_random (uint16_t max){
	return (rand()%max);
}

static uint16_t convert_data (double data_r, double data_l, double * velocidad_d, double * velocidad_r){
	if((data_l<=100) && (data_l>=-100) && (data_r<=100) && (data_r>=-100)) //probamos que no se encuentre dentro del rango
	{
		*velocidad_d = ((data_l + data_r) / 200);
		*velocidad_r = ((data_l - data_r) / 200);	
		return 1;
	}
	else
		return 0;
}

static void magic (void){
	//no magic example
	double dvelocity, rvelocity;
	double *p_dvelocity = &dvelocity, *p_rvelocity = &rvelocity;
	if(convert_data (percentage_new.right, percentage_new.left, p_dvelocity, p_rvelocity)){
		percentage_old.right = percentage_new.right;
		percentage_old.left = percentage_new.left;
		W_configureRobot (dvelocity, rvelocity);
	}


}

void S_setActuatorMov(uint16_t actuator_id, int16_t actuator_percentage){
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
	
}

void S_Init (void){
	init_random();
	percentage_old.right = 0;
	percentage_old.left = 0;

}