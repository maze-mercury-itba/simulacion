#ifndef VECTOR_H
#define	VECTOR_H

#include <stdint.h>


//tipos de datos punto (dos coordenadas cartesianas) y vector (dos puntos) con distintos tipos de datos

//ESTO ES UN BORRADOR MALE NO ME RETES

typedef struct { float x, y;} fpoint_t;

typedef struct { fpoint_t start, end; } fvector_t;


typedef struct { uint16_t x, y;} uipoint_t;

typedef struct { uipoint_t start, end; } uivector_t;

typedef struct { int16_t x, y;} ipoint_t;

typedef struct { ipoint_t start, end; } ivector_t;

typedef struct
{
	double x;
	double y;
}Point_t;


#endif //VECTOR_H