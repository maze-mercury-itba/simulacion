#include "World.h"
#include <cmath>
#define W_PI       3.14159265358979323846
#define MAX_HIPO	1000
#define W_ERROR	-1
#define DEFASAJE	1

typedef unsigned int uint;
static map_t myMap;
static robot_t myRobot;

static dpoint_t getNextHipoteticRobotPosition(void);//BIEN
static uint16_t hasCrashedWithWall(dpoint_t previousPoint, dpoint_t nextPoint);//BIEN
static bool	onSegment(dpoint_t p, dpoint_t q, dpoint_t r); //BIEN
static int orientation(dpoint_t p, dpoint_t q, dpoint_t r); //BIEN
static bool doIntersect(dpoint_t p1, dpoint_t q1, dpoint_t p2, dpoint_t q2);//BIEN
static double max(double x, double y);//BIEN
static double min(double x, double y);//BIEN
static double calculatePendient(dpoint_t firstPoint, dpoint_t secondPoint);//BIEN
static dpoint_t getIntersectionPoint(dpoint_t start, dpoint_t end, dvector_t wall);//BIEN
static double getMinDistance(double _angle, uint16_t sensorID);
static dpoint_t getPoint(dpoint_t _point, double angle, double hipotenusa);
static double getDistance(dpoint_t A, dpoint_t B);//BIEN
static double getAngle(uint16_t sensorID);
static dpoint_t getSensorPointOnMap(double _angle, uint16_t sensorID);
static bool ifVertical(dpoint_t p1, dpoint_t p2);//BIEN
static bool ifHorizontal(dpoint_t p1, dpoint_t p2);//BIEN
static dpoint_t calculateNextPoint(dpoint_t point, dpoint_t nextPoint);//BIEN


int16_t W_Init(map_t * mapInfo)
{
	myMap = *mapInfo;
	return false;
}

void W_setRobotConfiguration(robot_t * _myRobot)
{
	myRobot = *_myRobot;
	myRobot.rotation = 0;
	return;
}

bool W_configureRobot(double _direction, double _velocity,double _rotation)
{
	myRobot.direction = _direction;
	myRobot.velocity = _velocity;
	myRobot.rotation = _rotation;
	return true;
}

position_t W_getRobotPosition(void) //Devuelve la esquina superior izquierda del robot
{
	position_t answer;
	answer.angle = myRobot.rotation;
	answer.position = myRobot.position;
	return answer;
}

//position_t W_getRobotPosition(void) //Devuelve el centro del robot
//{
//	position_t center;
//	center.angle = myRobot.direction;
//
//	PointsStructure_t robotPos;
//	robotPos = setPoints(myRobot.position);
//
//	dvector_t diagonal;
//	diagonal.start = robotPos.firstPoint;
//	diagonal.end = robotPos.lastPoint;
//
//	center.position = getIntersectionPoint(robotPos.secondPoint, robotPos.thirdPoint, diagonal);
//	return center;
//}

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
		dvector_t wallTouched = myMap.walls[hasCrashedWithWall(myRobot.position, nextPosition) - 1];
		return CRASHED;
	}
}

sensData_t W_getSensorData(uint16_t sensorID)
{
	sensData_t answer;
	answer.angle = myRobot.direction + getAngle(sensorID);
	answer.distance = getMinDistance(answer.angle, sensorID);
	return answer;
}

/////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////

//Funcion encargada de calular el proximo punto hipotetico del robot, en caso de que no hubiera paredes
static dpoint_t getNextHipoteticRobotPosition(void)
{
	double _direction = (W_PI / 2) - myRobot.direction;	//Solamente paso el angulo que esta con respecto del eje y hacia angulos con respecto del eje x asi se hacen mas facil las cuentas
	dpoint_t _nextPoint;
	_nextPoint.y = myRobot.position.y - sin(_direction) * myRobot.velocity;
	_nextPoint.x = myRobot.position.x + cos(_direction) * myRobot.velocity;
	return _nextPoint;
}

/*Funcione que devuelve un 0 si no choco con nada, o el numero de pared mas cercana con la que colisiona el Robot
IMPORTANTE:
El numero de pared con la que colisiona el robot arranca en 1 hasta nWalls+1
O sea, si el robot choca con la primer pared del arrgelo de paredes, esta funcion devuelve un 1, NO DEVUELVE UN 0!!!!!!
*/
static uint16_t hasCrashedWithWall(dpoint_t previousPoint, dpoint_t nextPoint)
{
	uint16_t nearWall = 0;
	double distance = MAX_HIPO;
	dvector_t wall2Check;
	for (uint i = 0; i < myMap.nWalls; i++)
	{
		wall2Check = myMap.walls[i];
		for (uint u = 0; u < myRobot.amountOfPoints; u++)
		{
			dpoint_t tempPoint = calculateNextPoint(myRobot.robotPoints[u], nextPoint);
			dpoint_t prevPoint = absolutePoint(myRobot.robotPoints[u]);
			if (doIntersect(prevPoint, tempPoint, wall2Check.start, wall2Check.end))
			{
				dpoint_t newPoint;
				if (ifVertical(prevPoint, tempPoint) && ifVertical(wall2Check.start, wall2Check.end))
					newPoint = tempPoint;
				else
					newPoint = getIntersectionPoint(prevPoint, tempPoint, myMap.walls[i]);
				if (getDistance(newPoint, prevPoint) < distance)
				{
					nearWall = i + 1;
					distance = getDistance(newPoint, prevPoint);
				}
			}
		}
	}
	if (distance == MAX_HIPO)
		nearWall = 0;
	return nearWall;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//set de funciones obtenidas de http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/ 
//Y ya probadas.
static bool onSegment(dpoint_t p, dpoint_t q, dpoint_t r)//Funcion que checkea si el punto q se encuentra dentro del caudrado q forma segmento que forman p con r
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
	double answer = deltaY / deltaX;
	return answer;
}

/*Funcion que devuelve en donde se intersectan 2 segmentos(recibe 2 puntos y forma un segmento y
devuelve el punto de interseccion con la Wall
IMPORTANTE: esta funcion no chequea si hubo interseccion o no, asume que la hubo, en caso de que no haya interseccion y 
esta funcion sea llamada, devuelve basura.
Recibe: 2 puntos los cuales generan un segmento, y una pared
Devuelve: el punto de interseccion.
*/
static dpoint_t getIntersectionPoint(dpoint_t start, dpoint_t end, dvector_t wall)
{
	dpoint_t answer;
	//Estos calculos fueron hechos previamente por IAN, solamente copie la formula final que me quedo
	if (end.x != start.x && wall.end.x != wall.start.x)
	{
		answer.x = ((start.y - calculatePendient(start, end)*start.x) - (wall.start.y - calculatePendient(wall.start, wall.end) * wall.start.x)) / 
					(calculatePendient(wall.start, wall.end) - calculatePendient(start, end));
		answer.y = calculatePendient(start, end) * answer.x + (start.y - calculatePendient(start, end)*start.x);
	}
	if(end.x == start.x && wall.end.x != wall.start.x)
	{
		answer.x = start.x;
		answer.y = (calculatePendient(wall.start, wall.end) * answer.x) + wall.start.y - (calculatePendient(wall.start, wall.end) * wall.start.x);
	}
	if (wall.end.x == wall.start.x && end.x != start.x)
	{
		answer.x = wall.start.x;
		answer.y = (calculatePendient(start, end) * answer.x) + start.y - (calculatePendient(start, end) * start.x);
	}
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
	if (sqrt(pow((B.x - A.x), 2) + pow((B.y - A.y), 2)) > MAX_HIPO)
		return 0;
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

static bool ifVertical(dpoint_t p1, dpoint_t p2)
{
	if (p1.x == p2.x)
		return true;
	return false;
}

static bool ifHorizontal(dpoint_t p1, dpoint_t p2)
{
	if (p1.y == p2.y)
		return true;
	return false;
}

dpoint_t absolutePoint(dpoint_t point)
{
	dpoint_t answer;
	answer.x = myRobot.position.x - point.y * (sin(myRobot.rotation)) + (point.x * cos(myRobot.rotation));
	answer.y = myRobot.position.y + point.y * (cos(myRobot.rotation)) + (point.x * sin(myRobot.rotation));
	return answer;
}

static dpoint_t calculateNextPoint(dpoint_t point ,dpoint_t nextPoint)
{
	dpoint_t answer;
	answer.x = nextPoint.x - point.y * (sin(myRobot.rotation)) + (point.x * cos(myRobot.rotation));
	answer.y = nextPoint.y + point.y * (cos(myRobot.rotation)) + (point.x * sin(myRobot.rotation));
	return answer;
}

/*void W_rotateRobot(double angle, dpoint_t pointInRobot)
{
	dpoint_t realPoint = absolutePoint(pointInRobot);
	double deltaA, deltaB;
	deltaA = myRobot.position.y - pointInRobot.y;
	deltaB = myRobot.position.x - pointInRobot.x;
	double c = sqrt(pow(deltaA, 2) + pow(deltaB, 2));
	double tita;
	double t = atan((deltaA) / (deltaB));
	if (deltaA > 0 && deltaB > 0)
		tita = (W_PI / 2) - t; //agrego pi sobre 2 - lo que estaba
	else if (deltaA < 0 && deltaB > 0)
		tita = (W_PI / 2) + t;
	else if (deltaA < 0 && deltaB < 0)
		tita = (3 / 2)*W_PI - t;
	else if (deltaA > 0 && deltaB < 0)
		tita = (3 / 2)*W_PI + t;
	double beta = tita + angle;
	myRobot.position.y = c*sin(beta) + pointInRobot.y;
	myRobot.position.x = c*cos(beta) + pointInRobot.x;

	for (uint i = 0; i < myRobot.amountOfPoints; i++)
	{
		double deltaY, deltaX;
		deltaY = myRobot.robotPoints[i].y - pointInRobot.y;
		deltaX = myRobot.robotPoints[i].x - pointInRobot.x;
		double c = sqrt(pow(deltaY, 2) + pow(deltaX, 2));
		double tita;
		double t = atan((deltaY) / (deltaX));
		if (deltaY > 0 && deltaX > 0)
			tita = (W_PI / 2) - t; //agrego pi sobre 2 - lo que estaba
		else if (deltaY < 0 && deltaX > 0)
			tita = (W_PI / 2) + t;
		else if (deltaY < 0 && deltaX < 0)
			tita = (3 / 2)*W_PI - t;
		else if (deltaY > 0 && deltaX < 0)
			tita = (3/2)*W_PI + t;
		double beta = tita + angle;
		myRobot.robotPoints[i].y = c*sin(beta) + pointInRobot.y;
		myRobot.robotPoints[i].x = c*cos(beta) + pointInRobot.x;
		//double gamma;
		/*if (deltaY >= 0 && deltaX >= 0)
			gamma = -(tita + angle);
		else if (deltaY > 0 && deltaX < 0)
			gamma = W_PI - tita - angle;
		else if (deltaY < 0 && deltaX > 0)
			gamma = - tita - angle;
		else if(deltaY < 0 && deltaX < 0)
			gamma = W_PI - (tita + angle);*/

		/*myRobot.robotPoints[i].x = realPoint.x + cos(gamma)*c;
		myRobot.robotPoints[i].y = realPoint.y + sin(gamma)*c;
	}
	myRobot.rotation = myRobot.rotation + angle;
}*/
