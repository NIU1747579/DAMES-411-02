#ifndef TAULER_H
#define TAULER_H

#include <iostream>
#include <vector>
#include <memory>
#include <string>
#include <fstream>
using namespace std;

#include "posicio.hpp"
#include "Fitxa.h"
#include "info_joc.hpp"
#include "Moviment.h"

class Tauler
{
public:
	Tauler();
	~Tauler();

	void inicialitza(const string& nomFitxer);
	void actualitzaMovimentsValids();
	bool mouFitxa(const Posicio& origen, const Posicio& desti);
	string toString() const; //mostrar en pantalla lestat actual del tauler

	void getPosicionsPossibles(const Posicio& origen, vector<Posicio>& posicionsPossibles);
	bool esMovimentValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, bool& canvi);

	bool comprovarFiPartida(ColorFitxa& guanyador) const;

	void visualitzaTauler(const Posicio& fitxaSeleccionada, const vector<Posicio>& movimentsValids);
	void visualitzaMovimentsValids(const vector<Posicio>& posicions) const;

	// No se si estaria correcte pero
	Fitxa* getFitxa(int fila, int col) const;

	bool buscarCapturesRecursivas(const Posicio& origen, vector<Posicio>& posicionsPossibles);
	void buscarCapturesAux(const Posicio& pos, bool posicionesProcesadas[N_FILES][N_COLUMNES],
		vector<Posicio>& posicionsPossibles, bool& hiHaCaptures);
	void assignaFitxa(char tipusFitxa, const Posicio& posicio);
	bool esDestiDinsLimits(int fila, int col);
	bool movimentFitxaNormalValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, const Fitxa& fitxaOrigen, bool& esCaptura);
	bool movimentDamaValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, const Fitxa& fitxaOrigen, bool& esCaptura);
	int calculaDireccio(int diferencia) const;


private:
	Fitxa* m_tauler[N_FILES][N_COLUMNES];
};

#endif
