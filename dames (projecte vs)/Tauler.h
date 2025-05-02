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
	bool esMovimentValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, bool& canvi);
	string toString() const; //mostrar en pantalla lestat actual del tauler

	bool buscarCapturesRecursivas(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]);
	void buscarCapturesAux(const Posicio& pos, bool posicionesProcesadas[N_FILES][N_COLUMNES], int& nPosicions, Posicio posicionsPossibles[], bool& hiHaCaptures);
	void assignaFitxa(char tipusFitxa, const Posicio& posicio);
	bool esDestiDinsLimits(int fila, int col);
	bool movimentFitxaNormalValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, const Fitxa& fitxaOrigen, bool& esCaptura);
	bool movimentDamaValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, const Fitxa& fitxaOrigen, bool& esCaptura);



private:
	Fitxa m_tauler[N_FILES][N_COLUMNES];

};

#endif
