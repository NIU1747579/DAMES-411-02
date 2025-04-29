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
	Fitxa() : tipus_Fitxa(TIPUS_EMPTY), color_Fitxa(COLOR_BLANC), m_numMoviments(0) {}

	void setTipus(const TipusFitxa& tipus) { tipus_Fitxa = tipus; }
	void setColor(const ColorFitxa& color) { color_Fitxa = color; }

	void convertirADama();
	bool esBuida() const { return (tipus_Fitxa == TIPUS_EMPTY); } //per comprovar si la casella esta buida - Per el metode movimentValid
	bool esContraria(const Fitxa& contrari) const;

	TipusFitxa getTipus() const { return tipus_Fitxa; }
	ColorFitxa getColor()const { return color_Fitxa; }

	int getNumMoviments() const { return m_numMoviments; };
	Moviment getMoviment(int index) const { return m_movimentsValids[index]; };

	void netejaMovimentsValids();
	void afegeixMovimentValid(const Moviment& moviment);

private:
	TipusFitxa tipus_Fitxa;
	ColorFitxa color_Fitxa;
	Moviment m_movimentsValids[20];
	int m_numMoviments;
};
