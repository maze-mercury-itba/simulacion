#include <stdio.h>
#include <stdint.h>
#include "FileHandler.h"

static double info[F_MAX_AMOUNT_OF_DATA];

static double F_getInfo (uint16_t order)
{
	if(order > F_MAX_AMOUNT_OF_DATA)
		return F_ERROR_WAYD;
	return info[order];
}


int16_t F_Startup (char * filename)
{
	uint16_t counter = 0;
	FILE *fp;
	fp = fopen(filename,"r");
	if(fp)
	{
		while(fscanf(fp,"%lf",&info[counter++]) != EOF && counter < F_MAX_AMOUNT_OF_DATA)
		{
		}

	}
	else
		return(F_ERROR_CANTOPEN);
	fclose(fp);
	return(F_OK);

}

uint16_t F_getBasicInfo (uint16_t order)
{
	if(order<11)
	{
		return F_getInfo(order);
	}
	else
		return F_ERROR_WAYD;
}


double F_getSensorType (uint16_t sensornumber)
{
	return F_getInfo((sensornumber*4)+9);
}

double F_getSensorXPos (uint16_t sensornumber)
{
	return F_getInfo((sensornumber*4)+10);
}

double F_getSensorYPos (uint16_t sensornumber)
{
	return F_getInfo((sensornumber*4)+11);
}

double F_getSensorAngle (uint16_t sensornumber)
{
	return F_getInfo((sensornumber*4)+12);
}


