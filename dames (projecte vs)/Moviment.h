#include <iostream>
using namespace std;

#include "Posicio.h"

class Moviment
{
public:
	//CADA FITXA TE UNS MOVIMENT VALIDS, ELS MOVIMENTS HAN DE SER DE FITXA
	Moviment() {};

	void setOrigen(const Posicio& o) { origen = o; }
	void setDesti(const Posicio& d) { desti = d; }
	void setFitxaCapturada(const Posicio& p) { fitxacapturada = p; }

	Posicio getOrigen() const { return origen; }
	Posicio getDesti() const { return desti; }
	Posicio getFitxaCapturada() const { return fitxacapturada; }

	//metodes per calcular les posicions dels moviments
	//Metode 1: diagonal dreta
	//Metode 2: diagonal esperra
	//Metode 3: diagonal inf dreta
	//Mtode 4: diagonal inf esq
private:
	Posicio origen;
	Posicio desti;
	Posicio fitxacapturada; //nomes volem guardar una captura per moviment
};