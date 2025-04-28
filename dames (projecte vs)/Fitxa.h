#include <iostream>
using namespace std;
//#include "Posicio.h"
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
	Fitxa() : tipus_Fitxa(TIPUS_EMPTY), color_Fitxa(COLOR_BLANC) {}

	void setTipus(const TipusFitxa& tipus) { tipus_Fitxa = tipus; }
	void setColor(const ColorFitxa& color) { color_Fitxa = color; }
	
	void calcularMovimentValids(); //????????????????

	void convertirADama();
	bool esBuida() const { return (tipus_Fitxa == TIPUS_EMPTY); } //per comprovar si la casella esta buida - Per el metode movimentValid
	bool esContraria(const Fitxa& contrari);

	TipusFitxa getTipus() const { return tipus_Fitxa; }
	ColorFitxa getColor()const { return color_Fitxa; }

	Moviment getMoviments()const { return moviments; }

	void setTipus(const TipusFitxa& tipus) { tipus_Fitxa = tipus; }
	void setColor(const ColorFitxa& color) { color_Fitxa = color; }

private:
	TipusFitxa tipus_Fitxa;
	ColorFitxa color_Fitxa;
	Moviment moviments;//mov valids de la fitxa
};
