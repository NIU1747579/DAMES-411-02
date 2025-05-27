#include <iostream>

#ifndef POSICIO_H
#define POSICIO_H

#include <string>
#include <fstream>
using namespace std;

const int N_FILES = 8;
const int N_COLUMNES = 8;

class Posicio
{
public:
	Posicio() : m_fila(0), m_columna(0) {}
	Posicio(int fila, int columna) { m_fila = fila, m_columna = columna; }
	Posicio(const string& posicioString);


	int getFila()const { return m_fila; }
	int getColumna()const { return m_columna; }
	bool esValida() const;
	void fromString(const string& posicio);
	string toString() const;
	bool operator==(const Posicio& posicio) const; //comparar dos posicions

private:
	int m_fila;
	int m_columna;
};

ostream& operator<<(ostream& out, const Posicio& pos);
ifstream& operator>>(ifstream& fitxer, Posicio& posicio);

#endif
