#include <iostream>
# include <vector>

#include "Moviment.h"
#include "GraphicManager.h"

using namespace std;

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
	Fitxa() : tipus_Fitxa(TIPUS_EMPTY), color_Fitxa(COLOR_BLANC){}
	Fitxa(const TipusFitxa& tipus, const ColorFitxa& color) { tipus_Fitxa = tipus, color_Fitxa = color; }
	void setTipus(const TipusFitxa& tipus) { tipus_Fitxa = tipus; }
	void setColor(const ColorFitxa& color) { color_Fitxa = color; }
	TipusFitxa getTipus() const { return tipus_Fitxa; }
	ColorFitxa getColor()const { return color_Fitxa; }

	bool esBuida() const { return (tipus_Fitxa == TIPUS_EMPTY); } // Per comprovar si la casella esta buida
	void netejaMovimentsValids();
	void afegeixMovimentValid(const Moviment& moviment);
	const vector<Moviment>& getMovimentValid(const Moviment& moviment){ m_movimentsValids.push_back(moviment); }
	void visualitzaFitxa(float x, float y) const;

private:
	TipusFitxa tipus_Fitxa;
	ColorFitxa color_Fitxa;
	std::vector<Moviment> m_movimentsValids;
};
