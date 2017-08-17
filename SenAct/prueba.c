#include <stdio.h>
#include "senact.h"

W_configureRobot (double dvelocity, double rvelocity){
	printf("Velocidad frontal:%fl \nVelocidad de giro:%fl\n\n\n", dvelocity, rvelocity);
}

int main(int argc, char const *argv[])
{
	S_Init();
	while (1)
	{
		int a,b;
		printf("Porcentaje motor Izquierdo:");
		scanf("%d",&a);
		printf("Porcentaje motor Derecho:");
		scanf("%d",&b);
		S_setActuatorMov(0,a);
		S_setActuatorMov(1,b);
		S_Update();
	}
}