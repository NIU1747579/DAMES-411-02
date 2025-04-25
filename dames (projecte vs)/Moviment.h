#include <iostream>
using namespace std;

#include "Posicio.h"

class Moviment
{
public:
	//CADA FITXA TE UNS MOVIMENT VALIDS, ELS MOVIMENTS HAN DE SER DE FITXA
	Moviment() : m_nCaptures(0) {}

	void setOrigen(const Posicio& o) { m_origen = o; }
	void setDesti(const Posicio& d) { m_desti = d; }
	void afegirCaptura(const Posicio& c);

	Posicio getOrigen() const { return m_origen; }
	Posicio getDesti() const { return m_desti; }
	int getNCaptures() const { return m_nCaptures; }
	Posicio getCaptures(int i) { return m_captures[i]; }
	//metode per comprovar des de movimentValids, si el mov. que es vol 
	//realitzar es diagonal
	bool esMovimentDiagonal() const;
	bool esDireccioCorrecta(TipusFitxa tipus, ColorFitxa color) const;
	bool esMovimentNormalValid(TipusFitxa tipus, ColorFitxa color) const;
	Posicio getPosicioCaptura() const; //retorna la posicio de la fitxa capturada

private:
	Posicio m_origen; //nomes volem guardar una captura per moviment
	Posicio m_desti;
	Posicio m_captures[12]; //max de captures en un moviment
	int m_nCaptures;
};