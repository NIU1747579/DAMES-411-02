#include <iostream>
#include <fstream>
using namespace std;

#include "Posicio.h"
#include "Fitxa.h"
#include "Moviment.h"

class Tauler
{
public:
	void inicialitza(const string& nomFitxer); 
	void actualitzaMovimentsValids() const; 
	bool mouFitxa(const Posicio& origen, const Posicio& desti); 
	void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]); 


	string ToString() const; //mostrar en pantalla lestat actual del tauler
	Posicio seleccioFitxa(); //per seleccionar la fitxa amb la que volem jugar - guardem la posicio en fitxaTriada
	



private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];
	Posicio fitxaTriada; 
	Moviment moviments;
};
