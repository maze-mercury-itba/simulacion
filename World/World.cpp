#include "World.h"
#include <math.h>
#define W_PI       3.14159265358979323846

static Point_s getNextHipoteticRobotPosition(void);

static Map_s myMap;
static Robot_s myRobot;



int16_t W_init(Map_s * mapInfo)
{
	myMap = *mapInfo;
	return false;
}

void W_setRobotConfiguration(Robot_s * _myRobot)
{
	myRobot = *_myRobot;
	return;
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

static Point_s getNextHipoteticRobotPosition(void)
{
	double _direction = W_PI - myRobot.direction;	//Solamente paso el angulo que esta con respecto del eje y hacia angulos con respecto del eje x asi se hacen mas facil las cuentas
	Point_s _nextPoint;
	_nextPoint.x = cos(_direction) * myRobot.velocity;
	_nextPoint.y = sin(_direction) * myRobot.velocity;
	return _nextPoint;
}

//Funcione que devuelve un 0 si no choco con nada, o el indici de la pared con la que colisiona.
static uint16_t hasCrashedWithWall(Point_s previousPoint, Point_s nextPoint)
{
	static uint16_t i;
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
//
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
*/
static Point_s getIntersectionPoint(Point_s start, Point_s end, Wall_s wall)
{
	Point_s answer;
	//Estos calculos fueron hechos previamente por IAN, solamente copie la formula final que me quedo
	answer.x = (wall.start.y - (calculatePendient(wall.start, wall.end) * wall.start.x)) / (calculatePendient(start, end)\
		- calculatePendient(wall.start, wall.end));
	answer.y = (calculatePendient(start, end) * answer.x);
	return answer;
}