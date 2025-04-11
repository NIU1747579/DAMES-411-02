#include <iostream>
using namespace std;
#include "Posicio.h"
#include "Moviment.h"

typedef enum 
{ 
	TIPUS_NORMAL, 
	TIPUS_DAMA, 
	TIPUS_EMPTY 
} TipusFitxa;

typedef enum 
{ 
	COLOR_NEGRE, 
	COLOR_BLANC, 
} ColorFitxa;

class Fitxa
{
public:
	Fitxa();
	Fitxa() {}
	void gestioMoviments() const; //????????????????
	void convertirADama();
	bool esBuida() const { return (tipus_Fitxa == TIPUS_EMPTY); } //per comprovar si la casella esta buida - Per el metode movimentValid

	TipusFitxa getTipus() const { return tipus_Fitxa; }
	ColorFitxa getColor()const { return color_Fitxa; }
	Posicio getPosicioActual()const { return posicio_Actual; }

	void setTipus(const TipusFitxa& tipus) { tipus_Fitxa = tipus; }
	void setColor(const ColorFitxa& color) { color_Fitxa = color; }
	//SET POSICIO?
	
private:
	TipusFitxa tipus_Fitxa;
	ColorFitxa color_Fitxa;
	Posicio posicio_Actual; // esta duplicado en la classe tauler y no sabemos q hacer
	Moviment movimentValids[100];//?
};

