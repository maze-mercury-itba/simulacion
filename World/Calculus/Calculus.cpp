#include "Calculus.h"
#include <cmath>
#define W_PI		3.14159265358979323846
#define MAX_HIPO	1000
#define DOUBLE_EQUALITY	0.000005

bool onSegment(dpoint_t p, dpoint_t q, dpoint_t r)//Funcion que checkea si el punto q se encuentra dentro del caudrado q forma segmento que forman p con r
{
	if (q.x <= max(p.x, r.x) && q.x >= min(p.x, r.x) &&
		q.y <= max(p.y, r.y) && q.y >= min(p.y, r.y))
		return true;
	return false;
}

int orientation(dpoint_t p, dpoint_t q, dpoint_t r)
{
	// See http://www.geeksforgeeks.org/orientation-3-ordered-points/
	// for details of below formula.
	double val = (double)((q.y - p.y) * (r.x - q.x) -
		(q.x - p.x) * (r.y - q.y));

	if (val == 0)
		return 0;  // colinear

	return (val > 0) ? 1 : 2; // clock or counterclock wise
}

bool doIntersect(dpoint_t p1, dpoint_t q1, dpoint_t p2, dpoint_t q2)	//Funcion que checkea si dos segmentos formados por p1-q1 y p2-q2 se intersectan.
{
	// Find the four orientations needed for general and
	// special cases
	double o1 = orientation(p1, q1, p2);
	double o2 = orientation(p1, q1, q2);
	double o3 = orientation(p2, q2, p1);
	double o4 = orientation(p2, q2, q1);

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

//bool doIntersect(dpoint_t p0, dpoint_t p1, dpoint_t p2, dpoint_t p3)
//{
//	double x0, y0, x1, y1, x2, y2, x3, y3;
//	double m1, m2;
//	double x, y;
//	bool answer;
//	x0 = p0.x;
//	y0 = p0.y;
//	x1 = p1.x;
//	y1 = p1.y;
//	x2 = p2.x;
//	y2 = p2.y;
//	x3 = p3.x;
//	y3 = p3.y;
//
//	if (x0 != x1)
//		m1 = calculatePendient(p0, p1);
//	if (x2 != x3)
//		m2 = calculatePendient(p2, p3);
//	if (x0 != x1 && x2 != x3)
//	{
//		if (m1 != m2)
//		{
//			x = (y2 - m2*x2 - y0 + m1*x0) / (m1 - m2);
//			y = m1*x + y0 - m1*x0;
//			answer = (isItInside(x2, y2, x3, y3, x, y)) && (isItInside(x0, y0, x1, y1, x, y));//ME FIJO QUE EL X Y EL Y OBTENIDOS ESTEN DENTRO DE LOS SEGMENTOS
//		}
//		else //SI LAS PENDIENTES SON LAS MISMAS
//			answer = false;
//	}
//	if (x0 == x1 && x2 != x3)
//	{
//		x = x0;
//		y = m2 * x + y2 - m2*x2;
//		answer = (isItInside(x2, y2, x3, y3, x, y));
//	}
//	else if (x0 != x1 && x2 == x3)
//	{
//		x = x2;
//		y = m1 * x + y0 - m1*x0;
//		answer = (isItInside(x0, y0, x1, y1, x, y));
//	}
//	else
//		answer = false;
//	return answer;
//}
//
//bool isItInside(double x0, double y0, double x1, double y1, double x, double y)
//{
//	if (x1 >= x0 && y1 >= y0)
//	{
//		if (x <= x1 && x >= x0 && y <= y1 && y >= y0)
//			return true;
//		else
//			return false;
//	}
//	else if (x1 <= x0 && y1 >= y0)
//	{
//		if (x <= x0 && x >= x1 && y <= y1 && y >= y0)
//			return true;
//		else
//			return false;
//	}
//	else if (x1 >= x0 && y1 <= y0)
//	{
//		if (x <= x1 && x >= x0 && y <= y0 && y >= y1)
//			return true;
//		else
//			return false;
//	}
//	else if (x1 <= x0 && y1 <= y0)
//	{
//		if (x <= x0 && x >= x1 && y <= y0 && y >= y1)
//			return true;
//		else
//			return false;
//	}
//	else
//		return false;
//}


double max(double x, double y)
{
	double answer;
	x > y ? answer = x : answer = y;
	return answer;
}

double min(double x, double y)
{
	double answer;
	x < y ? answer = x : answer = y;
	return answer;
}

double calculatePendient(dpoint_t firstPoint, dpoint_t secondPoint)
{
	double deltaX = secondPoint.x - firstPoint.x;
	double deltaY = secondPoint.y - firstPoint.y;
	double answer = deltaY / deltaX;
	return answer;
}

dpoint_t getIntersectionPoint(dpoint_t start, dpoint_t end, dvector_t wall)
{
	dpoint_t answer;
	//Estos calculos fueron hechos previamente por IAN, solamente copie la formula final que me quedo
	if ( !aproximatelyEqual(end.x, start.x) && !aproximatelyEqual(wall.end.x,wall.start.x))
	{
		answer.x = ((start.y - calculatePendient(start, end)*start.x) - (wall.start.y - calculatePendient(wall.start, wall.end) * wall.start.x)) /
			(calculatePendient(wall.start, wall.end) - calculatePendient(start, end));
		answer.y = calculatePendient(start, end) * answer.x + (start.y - calculatePendient(start, end)*start.x);
	}
	else if (aproximatelyEqual(end.x, start.x) && !aproximatelyEqual(wall.end.x, wall.start.x))
	{
		answer.x = start.x;
		answer.y = (calculatePendient(wall.start, wall.end) * answer.x) + wall.start.y - (calculatePendient(wall.start, wall.end) * wall.start.x);
	}
	else if (aproximatelyEqual(wall.end.x, wall.start.x) && !aproximatelyEqual(end.x, start.x))
	{
		answer.x = wall.start.x;
		answer.y = (calculatePendient(start, end) * answer.x) + start.y - (calculatePendient(start, end) * start.x);
	}
	return answer;
}

double getDistance(dpoint_t A, dpoint_t B)
{
	if (sqrt(pow((B.x - A.x), 2) + pow((B.y - A.y), 2)) > MAX_HIPO)
		return 0;
	return sqrt(pow((B.x - A.x), 2) + pow((B.y - A.y), 2));
}

bool ifVertical(dpoint_t p1, dpoint_t p2)
{
	if (p1.x == p2.x)
		return true;
	return false;
}

bool ifHorizontal(dpoint_t p1, dpoint_t p2)
{
	if (p1.y == p2.y)
		return true;
	return false;
}

dpoint_t getPoint(dpoint_t _point, double angle, double hipotenusa)
{
	dpoint_t newPoint;
	newPoint.x =(_point.x + (cos((W_PI / 2) - angle)*hipotenusa)); //Hago W_PI/2 - angle porque lo paso a coordenadas normales x e y
	newPoint.y =(_point.y - (sin((W_PI / 2) - angle)*hipotenusa));
	return newPoint;
}

dpoint_t absolutePointWRobot(dpoint_t point, robot_t myRobot)
{
	dpoint_t answer;
	answer.x = myRobot.position.x - point.y * (sin(myRobot.D_angle)) + (point.x * cos(myRobot.D_angle));
	answer.y = myRobot.position.y + point.y * (cos(myRobot.D_angle)) + (point.x * sin(myRobot.D_angle));
	return answer;
}

double absoluteValue(double value)
{
	value >= 0 ? value = value : value = -value;
	return value;
}

bool aproximatelyEqual(double x, double y)
{
	if (x >= (y + DOUBLE_EQUALITY) || x <= (y - DOUBLE_EQUALITY))
		return false;
	return true;
}