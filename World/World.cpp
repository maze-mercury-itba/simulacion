#include "World.h"
#include <cmath>
#define W_PI       3.14159265358979323846
#define MAX_HIPO	1000


typedef struct
{
	dpoint_t firstPoint, secondPoint, thirdPoint, lastPoint;
}PointsStructure_t;

static map_t myMap;
static robot_t myRobot;

static dpoint_t getNextHipoteticRobotPosition(void);//BIEN
static uint16_t hasCrashedWithWall(dpoint_t previousPoint, dpoint_t nextPoint);//BIEN (PONELE)
static bool	onSegment(dpoint_t p, dpoint_t q, dpoint_t r);
static int orientation(dpoint_t p, dpoint_t q, dpoint_t r);
static bool doIntersect(dpoint_t p1, dpoint_t q1, dpoint_t p2, dpoint_t q2);//BIEN
static double max(double x, double y);//BIEN
static double min(double x, double y);//BIEN
static double calculatePendient(dpoint_t firstPoint, dpoint_t secondPoint);//BIEN
static dpoint_t getIntersectionPoint(dpoint_t start, dpoint_t end, dvector_t wall);//BIEN
static double getMinDistance(double _angle, uint16_t sensorID);
static dpoint_t getPoint(dpoint_t _point,double angle, double hipotenusa);
static double getDistance(dpoint_t A, dpoint_t B);
static double getAngle(uint16_t sensorID);
static dpoint_t getSensorPointOnMap(double _angle, uint16_t sensorID);
static PointsStructure_t setPoints(dpoint_t firstPoint);//BIEN
static void moveRobotToTouchedWall(dvector_t wallTouched, dpoint_t prevPoint, dpoint_t nextPoint);//BIEN


int16_t W_Init(map_t * mapInfo)
{
	myMap = *mapInfo;
	return false;
}

void W_setRobotConfiguration(robot_t * _myRobot)
{
	myRobot = *_myRobot;
	return;
}

bool W_configureRobot(double _direction, double _velocity)
{
	myRobot.direction = _direction;
	myRobot.velocity = _velocity;
	return true;
}

position_t W_getRobotPosition(void)
{
	position_t answer;
	answer.angle = myRobot.direction;
	answer.position = myRobot.position;
	return answer;
}

robotState_t W_Update(void)
{
	dpoint_t nextPosition = getNextHipoteticRobotPosition();
	if (!hasCrashedWithWall(myRobot.position, nextPosition))
	{
		myRobot.position = nextPosition;
		if (myRobot.position.x == myMap.target.x && myRobot.position.y == myMap.target.y)
			return ARRIVED_2_TARGET;
		return NOTHING_HAPPENED;
	}
	else
	{
		dvector_t wallTouched = myMap.walls[hasCrashedWithWall(myRobot.position, nextPosition)-1];
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
static dpoint_t getNextHipoteticRobotPosition(void)
{
	double _direction = (W_PI/2) - myRobot.direction;	//Solamente paso el angulo que esta con respecto del eje y hacia angulos con respecto del eje x asi se hacen mas facil las cuentas
	dpoint_t _nextPoint;
	_nextPoint.x = myRobot.position.x + cos(_direction) * myRobot.velocity;
	_nextPoint.y = myRobot.position.y - sin(_direction) * myRobot.velocity;
	return _nextPoint;
}

/*Funcione que devuelve un 0 si no choco con nada, o el numero de pared mas cercana con la que colisiona el Robot
IMPORTANTE:
El numero de pared con la que colisiona el robot arranca en 1 hasta nWalls+1
O sea, si el robot choca con la primer pared del arrgelo de paredes, esta funcion devuelve un 1, NO DEVUELVE UN 0!!!!!!
*/
static uint16_t hasCrashedWithWall(dpoint_t previousPoint, dpoint_t nextPoint)
{
	PointsStructure_t prevPoints = setPoints(previousPoint);
	PointsStructure_t nextPoints = setPoints(nextPoint);
	uint16_t i;
	uint16_t nearWall = 0;
	double distance = MAX_HIPO;
	dvector_t wall2Check;
	for (i = 0;i < myMap.nWalls; i++)
	{
		wall2Check = myMap.walls[i];
		if (doIntersect(prevPoints.firstPoint, nextPoints.firstPoint, wall2Check.start, wall2Check.end))
		{
			dpoint_t newPoint = getIntersectionPoint(prevPoints.firstPoint, nextPoints.firstPoint, myMap.walls[i]);
			if (getDistance(newPoint, prevPoints.firstPoint) < distance)
			{
				nearWall = i+1;
				distance = getDistance(newPoint, prevPoints.firstPoint);
			}
		}
		if (doIntersect(prevPoints.secondPoint, nextPoints.secondPoint, wall2Check.start, wall2Check.end))
		{
			dpoint_t newPoint = getIntersectionPoint(prevPoints.secondPoint, nextPoints.secondPoint, myMap.walls[i]);
			if (getDistance(newPoint, prevPoints.secondPoint) < distance)
			{
				nearWall = i+1;
				distance = getDistance(newPoint, prevPoints.secondPoint);
			}
		}
		if (doIntersect(prevPoints.thirdPoint, nextPoints.thirdPoint, wall2Check.start, wall2Check.end))
		{
			dpoint_t newPoint = getIntersectionPoint(prevPoints.thirdPoint, nextPoints.thirdPoint, myMap.walls[i]);
			if (getDistance(newPoint, prevPoints.thirdPoint) < distance)
			{
				nearWall = i+1;
				distance = getDistance(newPoint, prevPoints.thirdPoint);
			}
		}
		if (doIntersect(prevPoints.lastPoint, nextPoints.lastPoint, wall2Check.start, wall2Check.end))
		{
			dpoint_t newPoint = getIntersectionPoint(prevPoints.lastPoint, nextPoints.lastPoint, myMap.walls[i]);
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
static bool onSegment(dpoint_t p, dpoint_t q, dpoint_t r)		//Funcion que checkea si el punto q se encuentra dentro del caudrado q forma segmento que forman p con r
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;
	return false;
}

static int orientation(dpoint_t p, dpoint_t q, dpoint_t r)
{
	// See http://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (int)((q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y));

	if (val == 0)
		return 0;  // colinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

static bool doIntersect(dpoint_t p1, dpoint_t q1, dpoint_t p2, dpoint_t q2)	//Funcion que checkea si dos segmentos formados por p1-q1 y p2-q2 se intersectan.
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
static double calculatePendient(dpoint_t firstPoint, dpoint_t secondPoint)
{
	double deltaX = secondPoint.x - firstPoint.x;
	double deltaY = secondPoint.y - firstPoint.y;
	return (deltaY / deltaX);
}

/*Funcion que devuelve en donde se intersectan 2 segmentos(recibe 2 puntos y forma un segmento y
devuelve el punto de interseccion con la Wall
IMPORTANTE: esta funcion no chequea si hubo interseccion o no, asume que la hubo, en caso de que no halla interseccion y 
esta funcion sea llamada, devuelve basura.
Recibe: 2 puntos los cuales generan un segmento, y una pared
Devuelve: el punto de interseccion.
*/
static dpoint_t getIntersectionPoint(dpoint_t start, dpoint_t end, dvector_t wall)
{
	dpoint_t answer;
	//Estos calculos fueron hechos previamente por IAN, solamente copie la formula final que me quedo
	answer.x = ((start.y - calculatePendient(start, end)*start.x) - (wall.start.y - calculatePendient(wall.start, wall.end) * wall.start.x))
		/ (calculatePendient(wall.start, wall.end) - calculatePendient(start, end));
	answer.y = calculatePendient(start,end) * answer.x + (start.y - calculatePendient(start, end)*start.x);
	return answer;
}

static double getMinDistance(double _angle, uint16_t sensorID)
{
	uint16_t i;
	dpoint_t firstPoint;
	firstPoint = getSensorPointOnMap(_angle,sensorID);
	dpoint_t secondPoint = getPoint(firstPoint, _angle, MAX_HIPO);
	dvector_t wall2Check;
	dvector_t nearestWall = myMap.walls[0];//inicializacion de nearestWall para que funcione el for
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

static dpoint_t getPoint(dpoint_t _point,double angle, double hipotenusa)
{
	dpoint_t newPoint;
	newPoint.x = (uint16_t)(_point.x + (cos((W_PI/2) - angle)*hipotenusa)); //Hago W_PI/2 - angle porque lo paso a coordenadas normales x e y
	newPoint.y = (uint16_t)(_point.y + (sin((W_PI/2) - angle)*hipotenusa));
	return newPoint;
}

static double getDistance(dpoint_t A, dpoint_t B)
{
	return sqrt(pow((B.x - A.x), 2) + pow((B.y - A.y), 2));
}

static double getAngle(uint16_t sensorID)
{
	return myRobot.sensorArray[sensorID].angle;
}

static dpoint_t getSensorPointOnMap(double _angle, uint16_t sensorID)
{
	/*dpoint_t cero;
	cero.x = 0;
	cero.y = 0;
	double radius = getDistance(cero,myRobot.sensorArray[sensorID].positionOnRobot);*/
	dpoint_t answer;
	answer.x = myRobot.position.x + cos(myRobot.direction) ;
	answer.y = myRobot.position.y - sin(myRobot.direction) ;
	return answer;
}

static PointsStructure_t setPoints(dpoint_t firstPoint)
{
	double direction = (W_PI / 2) - myRobot.direction;
	double direction2 = (W_PI / 2) - direction;
	PointsStructure_t newStructure;
	newStructure.firstPoint = firstPoint;

	newStructure.secondPoint.x = firstPoint.x + (myRobot.width * cos(myRobot.direction));
	newStructure.secondPoint.y = firstPoint.y - (myRobot.width * sin(myRobot.direction));

	newStructure.thirdPoint.x = firstPoint.x - myRobot.height * (sin(myRobot.direction));
	newStructure.thirdPoint.y = firstPoint.y - myRobot.height * (cos(myRobot.direction));

	newStructure.lastPoint.x = newStructure.thirdPoint.x + (myRobot.width * cos(myRobot.direction));
	newStructure.lastPoint.y = newStructure.thirdPoint.y - (myRobot.width * sin(myRobot.direction));
	return newStructure;
}

/*Funcion encargada de mover el robot hacia el punto maximo que se puede mover antes de colisionar con la pared
Recibe: la pared con la que colisiona, el punto del robot antes de moverse, y el proximo punto hipotetico
No devuelve nada, cambia la posision de robot.
*/
static void moveRobotToTouchedWall(dvector_t wallTouched, dpoint_t prevPoint, dpoint_t nextPoint)
{
	PointsStructure_t prevPoints = setPoints(prevPoint);
	PointsStructure_t nextPoints = setPoints(nextPoint);
	dpoint_t newPoint;
	dpoint_t answer;
	double distance = MAX_HIPO;
	double direction = (W_PI / 2) - myRobot.direction;
	double direction2 = (W_PI / 2) - direction;

	if (doIntersect(prevPoints.firstPoint, nextPoints.firstPoint, wallTouched.start, wallTouched.end))
	{
		newPoint = getIntersectionPoint(prevPoints.firstPoint, nextPoints.firstPoint, wallTouched);
		distance = getDistance(prevPoints.firstPoint, newPoint);
		answer = newPoint;
	}

	if (doIntersect(prevPoints.secondPoint, nextPoints.secondPoint, wallTouched.start, wallTouched.end))
	{
		newPoint = getIntersectionPoint(prevPoints.secondPoint, nextPoints.secondPoint, wallTouched);
		if (getDistance(prevPoints.secondPoint, newPoint) < distance)
		{
			distance = getDistance(prevPoints.secondPoint, newPoint);
			answer.x = newPoint.x - (myRobot.width * cos(myRobot.direction));
			answer.y = newPoint.y + (myRobot.width * sin(myRobot.direction));
		}
	}

	if (doIntersect(prevPoints.thirdPoint, nextPoints.thirdPoint, wallTouched.start, wallTouched.end))
	{
		newPoint = getIntersectionPoint(prevPoints.thirdPoint, nextPoints.thirdPoint, wallTouched);
		if (getDistance(prevPoints.thirdPoint, newPoint) < distance)
		{
			distance = getDistance(prevPoints.thirdPoint, newPoint);
			answer.x = newPoint.x + myRobot.height * (sin(myRobot.direction));
			answer.y = newPoint.y + myRobot.height * (cos(myRobot.direction));
		}
	}

	if (doIntersect(prevPoints.lastPoint, nextPoints.lastPoint, wallTouched.start, wallTouched.end))
	{
		newPoint = getIntersectionPoint(prevPoints.lastPoint, nextPoints.lastPoint, wallTouched);
		if (getDistance(prevPoints.lastPoint, newPoint) < distance)
		{
			distance = getDistance(prevPoints.lastPoint, newPoint);
			answer.x = newPoint.x - (myRobot.width * cos(myRobot.direction)) + myRobot.height * (sin(myRobot.direction));
			answer.y = newPoint.y + (myRobot.width * sin(myRobot.direction)) - myRobot.height * (cos(myRobot.direction));
		}
	}

	myRobot.position = answer;
	return;
}