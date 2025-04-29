#ifndef TAULER_H
#define TAULER_H

#include <iostream>
#include <string>
#include <fstream>
using namespace std;

#include "posicio.hpp"
#include "Fitxa.h"
#include "Moviment.h"

class Tauler
{
public:
	Tauler() {};
	void inicialitza(const string& nomFitxer);
	void actualitzaMovimentsValids();
	bool mouFitxa(const Posicio& origen, const Posicio& desti);

	void getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
	bool esMovimentValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti);
	void bufarFitxa(int fila, int columna);
	string toString() const; //mostrar en pantalla lestat actual del tauler

	void obtenirPosicionsPossibles(int fila, int col, Posicio posicions[], int& numPosicions);

private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];
	Posicio fitxaTriada;
	Moviment moviments;
};

#endif
