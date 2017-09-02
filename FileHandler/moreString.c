#include "moreString.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>

void toLowercase(char *  dest, char * origin, uint32_t max)
{
	uint32_t i = 0;

	while (origin[i] != 0 && i < max)
	//recorrer el string hasta el final o hasta haber copiado el maximo
	{
		dest[i] = tolower(origin[i]);	//copiar pasando a minuscula
		i++;
	}

	dest[i]	= 0;	//agregar el terminador
}


float getFloat(char * string, int32_t * errorFlag)
{
	int32_t error = TRUE;				//se comienza en true, si hay error pasa a false
	uint32_t i = 0,	pointFlag = FALSE;	//indice, flag de punto

	if ( string[i] == '-' ) //el primer caracter puede ser un menos
	{
		i++;	
	}
	
	if (string[i++] == 0)	//verificar que hay al menos un numero
	{
		error = FALSE;
	}

	while( ( string[i] != 0 ) && ( error == TRUE ) )
	{		
		if ( isdigit(string[i]) == FALSE )
		{
			//unica posibilidad si no es numero: que sea el punto, una vez
			if (string[i] == '.' && pointFlag == FALSE)
			{
				pointFlag = TRUE;
			}
			else
			{
				error = FALSE;
			}
		}
		i++;		//avanzar al prox. caracter
	}

	if (errorFlag != NULL) {
		*errorFlag = error;
	}

	return (float)atof(string);	//si hubo error, lo que se devuelve no tiene sentido
}


uint32_t getUnsInt(char * string, int32_t * errorFlag)
{
	uint32_t i = 0;
	int32_t error = TRUE;

	if(!isdigit(string[i++]))	//verificar que haya al menos un digito
	{
		error = FALSE;	
	}

	while( ( string[i] != 0 ) && ( error == TRUE ) )
	//verificar los caracteres hasta llegar al final o que haya error
	{		
		error =  isdigit(string[i]);
		i++;
	}
	
	if (errorFlag != NULL) {
		*errorFlag = error;
	}

	return (uint32_t)atoi(string); //convertir el string (si hay error se devuelve basura)
}


