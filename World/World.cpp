
#include "World.h"
#include <cmath>
#define W_PI       3.14159265358979323846
#define MAX_HIPO	100

static Map_s myMap;
static Robot_s myRobot;

static Point_s getNextHipoteticRobotPosition(void);
static uint16_t hasCrashedWithWall(Point_s previousPoint, Point_s nextPoint);
static bool	onSegment(Point_s p, Point_s q, Point_s r);
static int orientation(Point_s p, Point_s q, Point_s r);
static bool doIntersect(Point_s p1, Point_s q1, Point_s p2, Point_s q2);
static double max(double x, double y);
static double min(double x, double y);
static double calculatePendient(Point_s firstPoint, Point_s secondPoint);
static Point_s getIntersectionPoint(Point_s start, Point_s end, Wall_s wall);
static double getMinDistance(double _angle, uint16_t sensorID);
static Point_s getPoint(Point_s _point,double angle, double hipotenusa);
static double getDistance(Point_s A, Point_s B);
static double getAngle(uint16_t sensorID);
static Point_s getSensorPointOnMap(double _angle, uint16_t sensorID);


int16_t W_Init(Map_s * mapInfo)
{
	myMap = *mapInfo;
	return false;
}


void W_setRobotConfiguration(Robot_s * _myRobot)
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


Point_s W_getRobotPosition(void)
{
	return myRobot.position;
}


int16_t W_Update(void)
{
	Point_s nextPosition = getNextHipoteticRobotPosition();
	if (!hasCrashedWithWall(myRobot.position, nextPosition))
	{
		myRobot.position = nextPosition;
		return true;
	}
	else
	{
		Wall_s wallTouched = myMap.walls[hasCrashedWithWall(myRobot.position, nextPosition)];
		myRobot.position = getIntersectionPoint(myRobot.position, nextPosition, wallTouched);
		return false;
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

static Point_s getNextHipoteticRobotPosition(void)
{
	double _direction = (W_PI/2) - myRobot.direction;	//Solamente paso el angulo que esta con respecto del eje y hacia angulos con respecto del eje x asi se hacen mas facil las cuentas
	Point_s _nextPoint;
	_nextPoint.x = (uint16_t)(cos(_direction) * myRobot.velocity);
	_nextPoint.y = (uint16_t)(sin(_direction) * myRobot.velocity);
	return _nextPoint;
}

//Funcione que devuelve un 0 si no choco con nada, o el indici de la pared con la que colisiona.
static uint16_t hasCrashedWithWall(Point_s previousPoint, Point_s nextPoint)
{
	uint16_t i;
	bool exit = false;
	Wall_s wall2Check;
	for (i = 0;(i < myMap.nWalls) && (exit == false); i++)
	{
		wall2Check = myMap.walls[i];
		if (doIntersect(previousPoint, nextPoint, wall2Check.start, wall2Check.end))
			exit = true;
	}
	if (i == (myMap.nWalls - 1))
		i = 0;
	return i;
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////
//set de funciones obtenidas de http://www.geeksforgeeks.org/check-if-two-given-line-segments-intersect/ 
//Y ya probadas supuestamente.
static bool onSegment(Point_s p, Point_s q, Point_s r)		//Funcion que checkea si el punto q se encuentra dentro del caudrado q forma segmento que forman p con r
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;
	return false;
}

static int orientation(Point_s p, Point_s q, Point_s r)
{
	// See http://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	int val = (q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y);

	if (val == 0)
		return 0;  // colinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

static bool doIntersect(Point_s p1, Point_s q1, Point_s p2, Point_s q2)	//Funcion que checkea si dos segmentos formados por p1-q1 y p2-q2 se intersectan.
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
static double calculatePendient(Point_s firstPoint, Point_s secondPoint)
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
static Point_s getIntersectionPoint(Point_s start, Point_s end, Wall_s wall)
{
	Point_s answer;
	//Estos calculos fueron hechos previamente por IAN, solamente copie la formula final que me quedo
	answer.x = (uint16_t)((wall.start.y - (calculatePendient(wall.start, wall.end) * wall.start.x)) / (calculatePendient(start, end)\
		- calculatePendient(wall.start, wall.end)));
	answer.y = (uint16_t)(calculatePendient(start, end) * answer.x);
	return answer;
}


static double getMinDistance(double _angle, uint16_t sensorID)
{
	uint16_t i;
	Point_s firstPoint;
	//firstPoint.x = myRobot.position.x + myRobot.sensorArray[sensorID].positionOnRobot.x;
	//firstPoint.y = myRobot.position.y + myRobot.sensorArray[sensorID].positionOnRobot.y
	firstPoint = getSensorPointOnMap(_angle,sensorID);
	Point_s secondPoint = getPoint(firstPoint, _angle, MAX_HIPO);
	Wall_s wall2Check;
	Wall_s nearestWall = myMap.walls[0];//inicializacion de nearestWall para que funcione el for
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


static Point_s getPoint(Point_s _point,double angle, double hipotenusa)
{
	Point_s newPoint;
	newPoint.x = (uint16_t)(_point.x + (cos((W_PI/2) - angle)*hipotenusa)); //Hago W_PI/2 - angle porque lo paso a coordenadas normales x e y
	newPoint.y = (uint16_t)(_point.y + (sin((W_PI/2) - angle)*hipotenusa));
	return newPoint;
}


static double getDistance(Point_s A, Point_s B)
{
	return sqrt(pow((B.x - A.x), 2) + pow((B.y - A.y), 2));
}


static double getAngle(uint16_t sensorID)
{
	return myRobot.sensorArray[sensorID].angle;
}


static Point_s getSensorPointOnMap(double _angle, uint16_t sensorID)//PENSAR BIEN LAS CUENTAS!!!!!!!
{
	Point_s cero;
	cero.x = 0;
	cero.y = 0;
	double radius = getDistance(cero,myRobot.sensorArray[sensorID].positionOnRobot);
	Point_s answer;
	answer.x = myRobot.position.x + radius * cos((W_PI/2) - _angle);
	answer.y = myRobot.position.y + radius * sin((W_PI/2) - _angle);
	return answer;
}

