#ifndef SENACT_H
#define SENACT_H
//codigos de errores -> error.h
#define	SENACT_ERROR	1
#define SENACT_OK 	0
#include <stdint.h>

//typedef sensor_id uint16_t

// Sensores
double S_getStateValue (sensor_id);
//* void S_getStateSens (sensor_id);


// Actuadores
void S_setActuatorMov (uint16_t actuator_id, int16_t actuator_percentage);


// Inicializacion
void S_Init (void);

// Mantenimiento
uint16_t S_Update (void);

double S_getSensorAngle(int sensorId);
// Configuracion
//uint16_t S_setSensorError (sen_id, * void);
//uint16_t S_setActuatorError (sen_id, * void);



//double	S_getSensorAngle(int sensorId);
//double	S_getSensorXPos(int sensorId);
//double	S_getSensorYPos(int sensorId);




//los ids quedarian definidos segun el numero que tenian cuando se llamo a set config



#endif
