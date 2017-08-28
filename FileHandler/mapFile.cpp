#include "mapFile.h"
#include <fstream>
#include <iomanip>
#include <cctype>
#include <queue>
extern "C" {
#include "moreString.h"
}

using namespace std;

//#define TARGET_STRING	"TARGET: "

bool readVector(ifstream& f, dvector_t& v);
bool readPoint(ifstream&f, dpoint_t& p);

//ESTAS FUNCIONES NO VERIFICAN NADA Y ASUME QUE OFSTREAM ESTA ABIERTO, es solo para que sea facil cambiar como se escribe
void writePoint(ofstream& f, dpoint_t p); //separar coordenadas de un mismo punto
void writeVector(ofstream& f, dvector_t v);



bool readMap(const char * path, map_t& map)
{
	if (path == NULL)
		return false;

	string p(path);
	p.append(".txt");
	ifstream f(p);
	if (f.good() == false)
		return false;

	bool valid = readPoint(f, map.target);
	queue<dvector_t> q;

	while (valid == true && f.eof() == false) {
#ifdef WIN32
		f.ignore(2);	//ignoro el enter
#else
		f.ignore(1);
#endif
		if (f.eof() == false) {
			dvector_t v;
			valid = readVector(f, v);
			q.push(v);
		}
	}

	if (valid == true && q.size() > 0) {
		map.nWalls = (uint16_t)q.size();
		map.walls = new dvector_t [map.nWalls];

		for (unsigned int i = 0; i < map.nWalls; i++) {
			map.walls[i] = q.front();
			q.pop();
		}
	}

	f.close();
	return valid;
}

bool makeMap(const char * path, map_t map)
{
	if (path == NULL)
		return false;

	string p (path); //para poder modificar path
	p.append(".txt");
	ofstream f(p);
	if (f.good() == false)
		return false;

//	f.write(TARGET_STRING, sizeof(TARGET_STRING)/sizeof(char)-1); //no copio el terminador
	writePoint(f, map.target);

	for (unsigned int i = 0; i < map.nWalls; i++) {
#ifdef WIN32
		f << '\r' << '\n';						//separacion entre vectores: un enter
#else
		f << '\n';
#endif
		writeVector(f, map.walls[i]);
	}

	f.close();
	return true;
}

void writePoint(ofstream& f, dpoint_t p)
{
	f << p.x << ' ' << ',' << ' ' << p.y;
}

void writeVector(ofstream& f, dvector_t v)
{
	writePoint(f, v.start);
	f << ' ' << '-' << ' ';
	writePoint(f, v.end);
}


bool readVector(ifstream& f, dvector_t& v)
{ 
	bool valid = false;

	if (readPoint(f, v.start) == true) {
		f.ignore(3); //ignoro " - "
		if (f.eof() == false) {
			valid = readPoint(f, v.end); //si puedo leer bien el segundo punto,valid es true
		}
	}

	return valid;
}

bool readPoint(ifstream&f, dpoint_t& p)
{ 
	int32_t valid = false;
	char buffer[MAX_NUM_SIZE + 1] = {'\0'};	//dejo un lugar para el terminador
	unsigned int i = 0;

	do { 
		buffer[i] = f.get();
	} while (buffer[i] != ' ' && i++ < MAX_NUM_SIZE && f.eof() == false);
	//la separacion entre x e y esta determinada aca: un espacio

	if (i > 0 && i<MAX_NUM_SIZE && f.eof() == false) {
		buffer[i] = '\0';	//reemplazo el espacio por un terminador para poder usar buffer como string
		p.x = getFloat(buffer, &valid);

		f.ignore(2); //ignoro ", "
		i = 0;

		if (f.eof() == false && bool(valid) == true) {
			do {
				buffer[i] = f.get();
			} while ( (bool(isdigit(buffer[i])) == true || buffer[i] == '.' || buffer[i] == '-' ) && f.eof() == false && i++ <= MAX_NUM_SIZE);

			//ahora no me fijo que separa y del prox vector, solo me fijo donde no hay mas numero
			//acepto con coma pero no negativo

			if (isdigit(buffer[i]) == false && buffer[i] != '.' || buffer[i] == '-' || f.eof() == true) {	//me fijo si llegue a copiar algo que no sea parte de y
				if (f.eof() == false) {
					f.unget();		//no afecto a lo que separa los puntos entre si por las dudas
				}
				buffer[i] = '\0';
				p.y = getFloat(buffer, &valid);
			}
			else {
				valid = false;
			}
		}
	}

	return bool(valid);
}
