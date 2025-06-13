#ifndef MOVIMENT_H  
#define MOVIMENT_H

#include <iostream>
#include "posicio.hpp"
#include <vector>
using namespace std;

class Moviment
{
public:
	Moviment(){}
	Moviment(const string& origen, const string& desti);
	Posicio getUltimaPosicio() const;

	// Afegit
	Posicio getPosicioOrigen() const;
	Posicio getPosicioDesti() const;

	void afegirCaptura(const Posicio& captura) { m_captures.push_back(captura); }
	void afegirPosicio(const Posicio& pos) { m_posicions.push_back(pos); }
	Posicio getPosicio(int index) const { return m_posicions[index]; }
	int getNumPosicions() const;

private:
	string m_origen;
	string m_desti;
	vector<Posicio> m_posicions; 
	vector<Posicio> m_captures;
}; 

#endif
