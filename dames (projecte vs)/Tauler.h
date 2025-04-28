#ifndef TAULER_H
#define TAULER_H
#include <iostream>
#include <fstream>
using namespace std;

//#include "posicio.hpp"

#include "Posicio.h"
#include "Fitxa.h"
#include "Moviment.h"

class Tauler
{
public:
	Tauler() {};
	void inicialitza(const string& nomFitxer);
	void actualitzaMovimentsValids() const;
	bool mouFitxa(const Posicio& origen, const Posicio& desti);

	void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
	bool esMovimentValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti);
	void bufarFitxa(int fila, int columna);
	string ToString() const; //mostrar en pantalla lestat actual del tauler

private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];
	Posicio fitxaTriada;
	Moviment moviments;
};

#endif