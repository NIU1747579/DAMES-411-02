#include <iostream>
#include <fstream>
using namespace std;

int const N_FILES = 8;
int const N_COLUMNES = 8;

class Posicio
{
public:
	Posicio(): m_fila(0), m_columna(0) {}
	Posicio(int fila, int columna) { m_fila = fila; m_columna = columna; }
	bool operator==(const Posicio& posicio) const; //comparar dos posicions

	void fromString(const string& posicio); 
	string toString() const;

	int getFila()const { return m_fila; }
	int getColumna()const { return m_columna; }

private:
	int m_fila;
	int m_columna;
};

ofstream& operator<<(ofstream& fitxer, const Posicio& posicio);
ifstream& operator>>(ifstream& fitxer, Posicio& posicio);

