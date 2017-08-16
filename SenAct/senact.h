#ifndef SENACT_H
#define SENACT_H
//codigos de errores -> error.h
#define	SENACT_ERROR	1
#define SENACT_OK 	0
#include <stdint.h>

typedef sensor_id uint16_t

// Sensores
double S_getStateValue (sensor_id);
* void S_getStateSens (sensor_id);
uint16_t S_getAmountSen (void);

// Actuadores
uint16_t S_setActuatorMovUnproc (uint16_t actuator_id, int8_t actuator_percentage);
uint16_t S_getAmountAct (void);


// Mantenimiento
uint16_t S_Update (void);

// Configuracion
uint16_t S_setSensorError (sen_id, * void);
uint16_t S_setActuatorError (sen_id, * void);



//los ids quedarian definidos segun el numero que tenian cuando se llamo a set config



#endif
