#ifndef CUAMOVIMENT_H
#define CUAMOVIMENT_H

#include <iostream>
#include <string>
#include <fstream>
#include "info_joc.hpp"
#include "Moviment.h"
#include "posicio.hpp"

using namespace std;

struct NodeMoviment {
	Moviment moviment; // Dades del moviment
	NodeMoviment* seguent; // Punter al seguent node
};

class CuaMoviments {
public:
	CuaMoviments() : m_primer(nullptr), m_ultim(nullptr) {}
	~CuaMoviments() { neteja(); }

	// Neteja tots el moviments de la cua
	void neteja();
	// Afegeix un unic moviment al final de la cua
	void afegeixMoviment(const string& origen, const string& desti);
	Moviment treuMoviment();
	bool esBuida() const { return m_primer == nullptr; }
	void guardaMoviments(const string& nomFitxer) const;
	void carregaMoviments(const string& nomFitxer);

private:
	NodeMoviment* m_primer;
	NodeMoviment* m_ultim;
};

#endif
