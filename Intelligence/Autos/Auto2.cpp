
#include "Auto2.h"
#include "IntelligenceCalculus.h"
#include "../IntelligenceResources.h"
#include <stdbool.h>

static bool leftWallFound = false;
static bool frontWallFound = false;

static double checkPointsOnXAxis(const dpoint_t * allCoordinates);
static double getInclination(dpoint_t p0, dpoint_t p1);

void startAuto2(const dpoint_t * allCoordinates)
{
	double leftInclinationAverage = checkPointsOnXAxis(allCoordinates);
	//double frontInclinationAverage = checkPointsOnYAxis(allCoordinates);
	if (!frontWallFound)
	{
		if (leftWallFound)
			actOverInclination(leftInclinationAverage);
		/*else if (!leftWallFound)
			actOverInclination(frontInclinationAverage);*/
	}
	/*else
		actOverInclination(frontInclinationAverage);*/

	if (!leftWallFound)
		turnLeft();
	else if (!frontWallFound)
		moveFoward();
	else
		turnRight();
}

static double checkPointsOnXAxis(const dpoint_t * allCoordinates)
{
	int i = 0, ammountOfInclinationsFound = 0;
	double inclination = 0.0, support = 0.0;
	bool wallFound = false;
	for (i=0; i < SENSORS_AMMOUNT; i++)
	{
		inclination += support;
		if (allCoordinates[i].x < 0)
		{
			wallFound = true;
			int u = i;
			i++;
			for (; (i < SENSORS_AMMOUNT) && (allCoordinates[i].x >= 0); i++);
			if (i == SENSORS_AMMOUNT - 1)
				break;
			support = getInclination(allCoordinates[u], allCoordinates[i]);
			ammountOfInclinationsFound++;
			i--;
		}
	}
	if (wallFound)
		leftWallFound = true;
	return (inclination/ammountOfInclinationsFound);
}

static double getInclination(dpoint_t p0, dpoint_t p1)
{
	double deltaX = p0.x - p1.x;
	double deltaY = p0.y - p1.y;
	return (deltaX / deltaY);
}