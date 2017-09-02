#ifndef CALCULUS_H
#define CALCULUS_H
#include "../WorldStructures.h"

bool onSegment(dpoint_t p, dpoint_t q, dpoint_t r); //BIEN
int orientation(dpoint_t p, dpoint_t q, dpoint_t r); //BIEN
//bool isItInside(double x0, double y0, double x1, double y1, double x, double y);
bool doIntersect(dpoint_t p1, dpoint_t q1, dpoint_t p2, dpoint_t q2);//BIEN
double max(double x, double y);//BIEN
double min(double x, double y);//BIEN
double calculatePendient(dpoint_t firstPoint, dpoint_t secondPoint);//BIEN
dpoint_t getIntersectionPoint(dpoint_t start, dpoint_t end, dvector_t wall);//BIEN
double getDistance(dpoint_t A, dpoint_t B);//BIEN
bool ifVertical(dpoint_t p1, dpoint_t p2);//BIEN
bool ifHorizontal(dpoint_t p1, dpoint_t p2);//BIEN
dpoint_t getPoint(dpoint_t _point, double angle, double hipotenusa);//BIEN
dpoint_t absolutePointWRobot(dpoint_t point, robot_t myRobot);//BIEN
double absoluteValue(double value);//BIEN
bool aproximatelyEqual(double x, double y);

#endif // !CALCULUS_H