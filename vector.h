#ifndef VECTOR_H
#define	VECTOR_H

#ifdef __cplusplus
#include <cstdint>
#else
#include <stdint.h>
#endif // __cplusplus

//tipos de datos punto (dos coordenadas cartesianas) y vector (dos puntos) con distintos tipos de datos
//ESTO ES UN BORRADOR MALE NO ME RETES

//con double
typedef struct { double x, y; } dpoint_t;
typedef struct { dpoint_t start, end; } dvector_t;

//con float
typedef struct { float x, y;} fdpoint_t;
typedef struct { fdpoint_t start, end; } fvector_t;

//con unsigned int de 16 bits
typedef struct { uint16_t x, y;} uidpoint_t;
typedef struct { uidpoint_t start, end; } uivector_t;

//con signed int de 16 bits
typedef struct { int16_t x, y;} idpoint_t;
typedef struct { idpoint_t start, end; } ivector_t;

#endif //VECTOR_H