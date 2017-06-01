#ifndef INTELIGENCIA_H
#define INTELIGENCIA_H
#include <stdbool.h> 
#include <stdint.h>
#include "Senact.h"
#include "math.h"


//FEDE
//extern uint16_t sen_state_sens(uint16_t _sensorId);
//extern void act_mov(nextStep_s);

typedef enum { TURN_R, TURN_L, GO_FWRD, GO_BCK };

typedef enum { AUTO1, AUTO2, ASSISTED1, ASSISTED2, MANUAL };

typedef struct {
	int16_t action;
	int16_t velocity;//milisegundos
}NextStep_s;

//20 grados = 0.6981317 radianes
//typedef struct {
//	uint16_t Deg_0To20;
//	uint16_t Deg_21To40;
//	uint16_t Deg_41To60;
//	uint16_t Deg_61To80;
//	uint16_t Deg_81To100;
//	uint16_t Deg_101To120;
//	uint16_t Deg_121To140;
//	uint16_t Deg_141To160;
//	uint16_t Deg_161To180;
//	uint16_t Deg_181To200;
//
//	uint16_t Deg_201To220;
//	uint16_t Deg_221To240;
//	uint16_t Deg_241To260;
//	uint16_t Deg_261To280;
//	uint16_t Deg_281To300;
//	uint16_t Deg_301To320;
//	uint16_t Deg_321To340;
//	uint16_t Deg_341To359;
 //HACER HASTA 360grados

}Directions_s;

void I_Update(int16_t _intelligenceType); // va a usar las funciones getInfo, processData, whatDoIDoNext, y doAction.

int16_t I_setMode(uint16_t mode);

//devuelven codigos de error. manual podria mandar info sobre el tipo de control, si es manual asistido o solo manual,
//que inteligencia se usa para el manual asistido si tenemos mas de una (lo mismo para auto pero sin lo de asistido).

#endif //INTELIGENCIA_H