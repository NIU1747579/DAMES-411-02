#include <iostream>
using namespace std;

#include "Posicio.h"

class Moviment
{
public:
	//CADA FITXA TE UNS MOVIMENT VALIDS, ELS MOVIMENTS HAN DE SER DE FITXA
	Moviment() : m_nCaptures(0),  m_numCaptures(0) {}

	void setOrigen(const Posicio& o) { m_origen = o; }
	void setDesti(const Posicio& d) { m_desti = d; }
	void afegirCaptura(const Posicio& c);

	int getNMoviments() const { return nMovimentsValids; }
	Moviment getMoviment(int index) const { return m_movimentsValids[index]; }

	Posicio getOrigen() const { return m_origen; }
	Posicio getDesti() const { return m_desti; }
	int getNCaptures() const { return m_nCaptures; }
	Posicio getCaptures(int i) { return m_captures[i]; }
	//metode per compr ovar des de movimentValids, si el mov. que es vol 
	//bool esMovimentDiagonal() const;

	Posicio getPosicioCaptura() const; //retorna la posicio de la fitxa capturada

	Posicio getUltimaPosicio() const;
	void afegirCaptura(const Posicio& captura);
	void afegirPosicio(const Posicio& pos);
	Posicio getPosicio(int index) const { return m_posicions[index]; };
	int getNumPosicions() const { return m_numPosicions; };

private:
	//nose si cal
	Posicio m_origen; //nomes volem guardar una captura per moviment
	Posicio m_desti;

	Posicio m_captures[12]; //max de captures en un moviment
	int m_numCaptures;

	Posicio m_posicions[20]; 
	int m_nPosicions;

	

};

