#pragma once
#include "../World/World.h"

//los paths que recibe son sin extension! le va a agregar .map

/*
	formato de los mapas:

-----------------------------------------------------------------------------------------------------
x , y	//x, y: coordenadas del target

0 , 0 - 0 , 255			//paredes
100 , 200 - 500 , 5
-------------------------------------------------------------------------------------------------------
-sin tabs
-la unica linea en blanco es entre target y la primera pared
-los ----- son solo para mostrar donde empieza y termina el archivo, NO HAY QUE PONERLOS, mismo con los comentarios
-los numeros pueden ser float, separando la parte entera de la decimal con un punto. solo digitos, punto! siempre positivos!
-coordenadas x e y de un mismo punto van separadas por " , ", siempre. primero x y despues y
-punta y origen de un mismo vector van separadas por " - " siempre. primero origen y despues punta
-cada numero no puede tener mas de MAX_NUM_SIZE caracteres, contando el punto y el signo si los tuviera

*/
#define MAX_NUM_SIZE	15

bool readMap(const char * path, map_t& map);
//lee el mapa contenido en el archivo cuyo nombre recibe como parametro, y guarda los datos en el 
//mapa que recibe por referencia. 
//devuelve false si no pudo abrir el archivo o si el mismo no tenia formato valido
//cuando deje de usar el mapa, llamar a delete[] map.walls!!

bool makeMap(const char * path, map_t map);
//realiza el proceso inverso a readMap: a partir del mapa que recibe, crea un archivo con el path especificado
//que contenga la info sobre ese mapa, y que despues se va a poder leer con readMap
//devuelve false si no pudo crear el archivo o si el mapa no era valido
//SI YA EXISTIA UN ARCHIVO CON ESTE NOMBRE LO SOBREESCRIBE!!