#ifndef CUAMOVIMENT_H
#define CUAMOVIMENT_H

#include <iostream>
#include <string>
#include <fstream>
#include "info_joc.hpp"
#include "posicio.hpp"
#include "Moviment.h"

using namespace std;

struct NodeMoviment {
	Moviment moviment; // Dades del moviment
	NodeMoviment* seguent; // Punter al seguent node

};

class CuaMoviments {
public:
	CuaMoviments() : m_primer(nullptr), m_ultim(nullptr) {}
	~CuaMoviments() { neteja(); }

	Moviment getUltim() const;
	Moviment getPrimer() const;

	void neteja();
	// Afegeix un unic moviment al final de la cua
	void afegeixMoviment(const string& origen, const string& desti);
	Moviment treuMoviment();
	bool esBuida() const { return m_primer == nullptr; }
	void guardaMoviments(const string& nomFitxer) const;

private:
	NodeMoviment* m_primer; 
	NodeMoviment* m_ultim;
};

#endif
