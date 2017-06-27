#include "World.h"
#include <cmath>
#define W_PI       3.14159265358979323846
#define MAX_HIPO	100


typedef struct
{
	uipoint_t firstPoint, secondPoint, thirdPoint, lastPoint;
}PointsStructure_t;

static Map_t myMap;
static Robot_t myRobot;

static uipoint_t getNextHipoteticRobotPosition(void);
static uint16_t hasCrashedWithWall(uipoint_t previousPoint, uipoint_t nextPoint);
static bool	onSegment(uipoint_t p, uipoint_t q, uipoint_t r);
static int orientation(uipoint_t p, uipoint_t q, uipoint_t r);
static bool doIntersect(uipoint_t p1, uipoint_t q1, uipoint_t p2, uipoint_t q2);
static double max(double x, double y);
static double min(double x, double y);
static double calculatePendient(uipoint_t firstPoint, uipoint_t secondPoint);
static uipoint_t getIntersectionPoint(uipoint_t start, uipoint_t end, Wall_t wall);
static double getMinDistance(double _angle, uint16_t sensorID);
static uipoint_t getPoint(uipoint_t _point,double angle, double hipotenusa);
static double getDistance(uipoint_t A, uipoint_t B);
static double getAngle(uint16_t sensorID);
static uipoint_t getSensorPointOnMap(double _angle, uint16_t sensorID);
static PointsStructure_t setPoints(uipoint_t firstPoint);
static void moveRobotToTouchedWall(Wall_t wallTouched, uipoint_t prevPoint, uipoint_t nextPoint);


int16_t W_Init(Map_t * mapInfo)
{
	myMap = *mapInfo;
	return false;
}

void W_setRobotConfiguration(Robot_t * _myRobot)
{
	myRobot = *_myRobot;
	return;
}

bool W_configureRobot(uint16_t _direction, uint16_t _velocity)
{
	if((_direction>0)&&(_direction<(2*W_PI)))
		myRobot.direction = _direction;
	else
		return false;
	myRobot.velocity = _velocity;
	return true;
}

RobotPosition_t W_getRobotPosition(void)
{
	RobotPosition_t answer;
	answer.angle = myRobot.direction;
	answer.position = myRobot.position;
	return answer;
}

RobotState_t W_Update(void)
{
	uipoint_t nextPosition = getNextHipoteticRobotPosition();
	if (!hasCrashedWithWall(myRobot.position, nextPosition))
	{
		myRobot.position = nextPosition;
		if (myRobot.position.x == myMap.target.x && myRobot.position.y == myMap.target.y)
			return ARRIVED_2_TARGET;
		return NOTHING_HAPPENED;
	}
	else
	{
		Wall_t wallTouched = myMap.walls[hasCrashedWithWall(myRobot.position, nextPosition)-1];
		moveRobotToTouchedWall(wallTouched,myRobot.position, nextPosition);
		return CRASHED;
	}
}

sensData_t W_getSensorData(uint16_t sensorID)
{
	sensData_t answer;
	answer.angle =  myRobot.direction + getAngle(sensorID);
	answer.distance = getMinDistance(answer.angle, sensorID);
	return answer;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

//Funcion encargada de calular el proximo punto hipotetico del robot, en caso de que no hubiera paredes
static uipoint_t getNextHipoteticRobotPosition(void)
{
	double _direction = (W_PI/2) - myRobot.direction;	//Solamente paso el angulo que esta con respecto del eje y hacia angulos con respecto del eje x asi se hacen mas facil las cuentas
	uipoint_t _nextPoint;
	_nextPoint.x = (uint16_t)(cos(_direction) * myRobot.velocity);
	_nextPoint.y = (uint16_t)(sin(_direction) * myRobot.velocity);
	return _nextPoint;
}

/*Funcione que devuelve un 0 si no choco con nada, o el numero de pared mas cercana con la que colisiona el Robot
IMPORTANTE:
El numero de pared con la que colisiona el robot arranca en 1 hasta nWalls+1
O sea, si el robot choca con la primer pared del arrgelo de paredes, esta funcion devuelve un 1, NO DEVUELVE UN 0!!!!!!
*/
static uint16_t hasCrashedWithWall(uipoint_t previousPoint, uipoint_t nextPoint)
{
	PointsStructure_t prevPoints = setPoints(previousPoint);
	PointsStructure_t nextPoints = setPoints(nextPoint);
	uint16_t i;
	uint16_t nearWall = 0;
	double distance = MAX_HIPO;
	Wall_t wall2Check;
	for (i = 0;i < myMap.nWalls; i++)
	{
		wall2Check = myMap.walls[i];
		if (doIntersect(prevPoints.firstPoint, nextPoints.firstPoint, wall2Check.start, wall2Check.end))
		{
			uipoint_t newPoint = getIntersectionPoint(prevPoints.firstPoint, nextPoints.firstPoint, myMap.walls[i]);
			if (getDistance(newPoint, prevPoints.firstPoint) < distance)
			{
				nearWall = i+1;
				distance = getDistance(newPoint, prevPoints.firstPoint);
			}
		}
		else if (doIntersect(prevPoints.secondPoint, nextPoints.secondPoint, wall2Check.start, wall2Check.end))
		{
			uipoint_t newPoint = getIntersectionPoint(prevPoints.secondPoint, nextPoints.secondPoint, myMap.walls[i]);
			if (getDistance(newPoint, prevPoints.secondPoint) < distance)
			{
				nearWall = i+1;
				distance = getDistance(newPoint, prevPoints.secondPoint);
			}
		}
		else if (doIntersect(prevPoints.thirdPoint, nextPoints.thirdPoint, wall2Check.start, wall2Check.end))
		{
			uipoint_t newPoint = getIntersectionPoint(prevPoints.thirdPoint, nextPoints.thirdPoint, myMap.walls[i]);
			if (getDistance(newPoint, prevPoints.thirdPoint) < distance)
			{
				nearWall = i+1;
				distance = getDistance(newPoint, prevPoints.thirdPoint);
			}
		}
		else if (doIntersect(prevPoints.lastPoint, nextPoints.lastPoint, wall2Check.start, wall2Check.end))
		{
			uipoint_t newPoint = getIntersectionPoint(prevPoints.lastPoint, nextPoints.lastPoint, myMap.walls[i]);
			if (getDistance(newPoint, prevPoints.lastPoint) < distance)
			{
				nearWall = i+1;
				distance = getDistance(newPoint, prevPoints.lastPoint);
			}
		}
	}
	if (distance == MAX_HIPO)
		nearWall = 0;
	return nearWall;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//set de funciones obtenidas de http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/ 
//Y ya probadas supuestamente.
static bool onSegment(uipoint_t p, uipoint_t q, uipoint_t r)		//Funcion que checkea si el punto q se encuentra dentro del caudrado q forma segmento que forman p con r
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;
	return false;
}

static int orientation(uipoint_t p, uipoint_t q, uipoint_t r)
{
	// See http://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0)
		return 0;  // colinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

static bool doIntersect(uipoint_t p1, uipoint_t q1, uipoint_t p2, uipoint_t q2)	//Funcion que checkea si dos segmentos formados por p1-q1 y p2-q2 se intersectan.
{
	// Find the four orientations needed for general and
	// special cases
	int o1 = orientation(p1, q1, p2);
	int o2 = orientation(p1, q1, q2);
	int o3 = orientation(p2, q2, p1);
	int o4 = orientation(p2, q2, q1);

	// General case
	if (o1 != o2 && o3 != o4)
		return true;
	// Special Cases
	// p1, q1 and p2 are colinear and p2 lies on segment p1q1
	if (o1 == 0 && onSegment(p1, p2, q1))
		return true;
	// p1, q1 and p2 are colinear and q2 lies on segment p1q1
	if (o2 == 0 && onSegment(p1, q2, q1))
		return true;
	// p2, q2 and p1 are colinear and p1 lies on segment p2q2
	if (o3 == 0 && onSegment(p2, p1, q2))
		return true;
	// p2, q2 and q1 are colinear and q1 lies on segment p2q2
	if (o4 == 0 && onSegment(p2, q1, q2))
		return true;
	
	return false; // Doesn't fall in any of the above cases
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////

/*Funcion que recibe 2 doubles y devuelve el maximo entre los 2*/
static double max(double x, double y)
{
	double answer;
	x > y ? answer = x : answer = y;
	return answer;
}

/*Analoga a max, pero devuelve el minimo*/
static double min(double x, double y)
{
	double answer;
	x < y ? answer = x : answer = y;
	return answer;
}

/*Funcion que recive 2 puntos y calcula la pendiente del segmeto formado, la devuelve como double
esto lo hace por medio de la division de deltax sobre deltay*/
static double calculatePendient(uipoint_t firstPoint, uipoint_t secondPoint)
{
	double deltaX = firstPoint.x - secondPoint.x;
	double deltaY = firstPoint.y - secondPoint.y;
	return (deltaX / deltaY);
}

/*Funcion que devuelve en donde se intersectan 2 segmentos(recibe 2 puntos y forma un segmento y
devuelve el punto de interseccion con la Wall
IMPORTANTE: esta funcion no chequea si hubo interseccion o no, asume que la hubo, en caso de que no halla interseccion y 
esta funcion sea llamada, devuelve basura.
Recibe: 2 puntos los cuales generan un segmento, y una pared
Devuelve: el punto de interseccion.
*/
static uipoint_t getIntersectionPoint(uipoint_t start, uipoint_t end, Wall_t wall)
{
	uipoint_t answer;
	//Estos calculos fueron hechos previamente por IAN, solamente copie la formula final que me quedo
	answer.x = (uint16_t)((wall.start.y - (calculatePendient(wall.start, wall.end) * wall.start.x)) / (calculatePendient(start, end)\
		- calculatePendient(wall.start, wall.end)));
	answer.y = (uint16_t)(calculatePendient(start, end) * answer.x);
	return answer;
}

static double getMinDistance(double _angle, uint16_t sensorID)
{
	uint16_t i;
	uipoint_t firstPoint;
	//firstPoint.x = myRobot.position.x + myRobot.sensorArray[sensorID].positionOnRobot.x;
	//firstPoint.y = myRobot.position.y + myRobot.sensorArray[sensorID].positionOnRobot.y
	firstPoint = getSensorPointOnMap(_angle,sensorID);
	uipoint_t secondPoint = getPoint(firstPoint, _angle, MAX_HIPO);
	Wall_t wall2Check;
	Wall_t nearestWall = myMap.walls[0];//inicializacion de nearestWall para que funcione el for
	for (i = 0; i < myMap.nWalls; i++)
	{
		wall2Check = myMap.walls[i];
		if (doIntersect(firstPoint, secondPoint, wall2Check.start, wall2Check.end))
		{
			if(getDistance(firstPoint,getIntersectionPoint(firstPoint,secondPoint,wall2Check))\
				<= getDistance(firstPoint,getIntersectionPoint(firstPoint,secondPoint, nearestWall)))
				nearestWall = wall2Check;
		}		
	}
	return getDistance(firstPoint,getIntersectionPoint(firstPoint,secondPoint,nearestWall));
}

static uipoint_t getPoint(uipoint_t _point,double angle, double hipotenusa)
{
	uipoint_t newPoint;
	newPoint.x = (uint16_t)(_point.x + (cos((W_PI/2) - angle)*hipotenusa)); //Hago W_PI/2 - angle porque lo paso a coordenadas normales x e y
	newPoint.y = (uint16_t)(_point.y + (sin((W_PI/2) - angle)*hipotenusa));
	return newPoint;
}

static double getDistance(uipoint_t A, uipoint_t B)
{
	return sqrt(pow((B.x - A.x), 2) + pow((B.y - A.y), 2));
}

static double getAngle(uint16_t sensorID)
{
	return myRobot.sensorArray[sensorID].angle;
}

static uipoint_t getSensorPointOnMap(double _angle, uint16_t sensorID)//PENSAR BIEN LAS CUENTAS!!!!!!!
{
	uipoint_t cero;
	cero.x = 0;
	cero.y = 0;
	double radius = getDistance(cero,myRobot.sensorArray[sensorID].positionOnRobot);
	uipoint_t answer;
	answer.x = myRobot.position.x + radius * cos((W_PI/2) - _angle);
	answer.y = myRobot.position.y + radius * sin((W_PI/2) - _angle);
	return answer;
}

static PointsStructure_t setPoints(uipoint_t firstPoint)
{
	double direction = (W_PI / 2) - myRobot.direction;
	double direction2 = (W_PI / 2) - direction;
	PointsStructure_t newStructure;
	newStructure.firstPoint = firstPoint;

	newStructure.secondPoint.x = firstPoint.x + (myRobot.width * cos(direction));
	newStructure.secondPoint.y = firstPoint.y + (myRobot.width * sin(direction));

	newStructure.thirdPoint.x = firstPoint.x + (myRobot.height * cos(direction2));
	newStructure.thirdPoint.y = firstPoint.y - (myRobot.height * sin(direction2));

	newStructure.lastPoint.x = newStructure.thirdPoint.x + (myRobot.width * cos(direction));
	newStructure.lastPoint.y = newStructure.thirdPoint.y + (myRobot.width * sin(direction));
	return newStructure;
}

/*Funcion encargada de mover el robot hacia el punto maximo que se puede mover antes de colisionar con la pared
Recibe: la pared con la que colisiona, el punto del robot antes de moverse, y el proximo punto hipotetico
No devuelve nada, cambia la posision de robot.
*/
static void moveRobotToTouchedWall(Wall_t wallTouched, uipoint_t prevPoint, uipoint_t nextPoint)
{
	PointsStructure_t prevPoints = setPoints(prevPoint);
	PointsStructure_t nextPoints = setPoints(nextPoint);
	uipoint_t newPoint;
	double direction = (W_PI / 2) - myRobot.direction;
	double direction2 = (W_PI / 2) - direction;

	if (doIntersect(prevPoints.firstPoint, nextPoints.firstPoint, wallTouched.start, wallTouched.end))
	{
		newPoint = getIntersectionPoint(prevPoints.firstPoint, nextPoints.firstPoint, wallTouched);
		myRobot.position = newPoint;
	}

	else if (doIntersect(prevPoints.secondPoint, nextPoints.secondPoint, wallTouched.start, wallTouched.end))
	{
		newPoint = getIntersectionPoint(prevPoints.secondPoint, nextPoints.secondPoint, wallTouched);
		myRobot.position.x = newPoint.x - (myRobot.width * cos(direction));
		myRobot.position.y = newPoint.y - (myRobot.width * sin(direction));
	}

	else if (doIntersect(prevPoints.thirdPoint, nextPoints.thirdPoint, wallTouched.start, wallTouched.end))
	{
		newPoint = getIntersectionPoint(prevPoints.thirdPoint, nextPoints.thirdPoint, wallTouched);
		myRobot.position.x = newPoint.x - (myRobot.height * cos(direction2));
		myRobot.position.y = newPoint.y + (myRobot.height * sin(direction2));
	}

	else if (doIntersect(prevPoints.lastPoint, nextPoints.lastPoint, wallTouched.start, wallTouched.end))
	{
		newPoint = getIntersectionPoint(prevPoints.lastPoint, nextPoints.lastPoint, wallTouched);
		myRobot.position.x = newPoint.x - (myRobot.height * cos(direction2)) - (myRobot.width * cos(direction));
		myRobot.position.y = newPoint.y + (myRobot.height * sin(direction2)) - (myRobot.width * sin(direction));
	}
	return;
}