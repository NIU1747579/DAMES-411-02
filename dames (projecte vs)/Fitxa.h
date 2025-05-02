#include <iostream>
using namespace std;

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
	Fitxa() : m_numMoviments(0), tipus_Fitxa(TIPUS_EMPTY){}

	void setTipus(const TipusFitxa& tipus) { tipus_Fitxa = tipus; }
	void setColor(const ColorFitxa& color) { color_Fitxa = color; }
	TipusFitxa getTipus() const { return tipus_Fitxa; }
	ColorFitxa getColor()const { return color_Fitxa; }

	bool esBuida() const { return (tipus_Fitxa == TIPUS_EMPTY); } // Per comprovar si la casella esta buida
	void netejaMovimentsValids();
	void afegeixMovimentValid(const Moviment& moviment);

private:
	TipusFitxa tipus_Fitxa;
	ColorFitxa color_Fitxa;
	Moviment m_movimentsValids[20];
	int m_numMoviments;
};
