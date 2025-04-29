#ifndef MOVIMENT_H  
#define MOVIMENT_H

#include <iostream>
using namespace std;

#include "posicio.hpp"

class Moviment
{
public:
	Moviment() : m_numCaptures(0), m_numPosicions(0) {}

	void setOrigen(const Posicio& o) { m_origen = o; }
	void setDesti(const Posicio& d) { m_desti = d; }

	Posicio getOrigen() const { return m_origen; }
	Posicio getDesti() const { return m_desti; }
	int getNCaptures() const { return m_numCaptures; }
	Posicio getCaptures(int i) { return m_captures[i]; }

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
	int m_numPosicions;
};

#endif
