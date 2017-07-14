#ifndef INTELIGENCIA_H
#define INTELIGENCIA_H
#include <stdbool.h> 
#include <stdint.h>
#include "../senact/senact.h"

//FEDE
//extern uint16_t sen_state_sens(uint16_t _sensorId);
//extern void act_mov(nextStep_s);

typedef enum{TURN_R,TURN_L,GO_FWRD,GO_BCK} direction_t;

typedef enum{AUTO1, AUTO2, ASSISTED1, ASSISTED2, MANUAL} intMode_t;

typedef struct {
	int16_t action;
	int16_t velocity;//milisegundos
}nextStep_t;

void I_Init(uint16_t mode); //configuracion inicial

void I_Update(void); // va a usar las funciones getInfo, processData, whatDoIDoNext, y doAction.

void I_Drive(void * estructura); //recibe lo que llega del joystick o lo que sea

int16_t I_setMode(uint16_t mode); 

//devuelven codigos de error. manual podria mandar info sobre el tipo de control, si es manual asistido o solo manual,
//que inteligencia se usa para el manual asistido si tenemos mas de una (lo mismo para auto pero sin lo de asistido).

#endif //INTELIGENCIA_H