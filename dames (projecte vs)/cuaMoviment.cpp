#include "cuaMoviment.h"

void CuaMoviments::neteja() {
 	while (!esBuida()) {
		treuMoviment();
	}
}

Moviment CuaMoviments::getPrimer() const {
	if (m_primer != nullptr) {
		return m_primer->moviment;
	}
	return Moviment();
}

Moviment CuaMoviments::getUltim() const {
	if (m_ultim != nullptr) {
		return m_ultim->moviment;
	}
	return Moviment();
}

void CuaMoviments::afegeixMoviment(const string& origen, const string& desti)
{
	Moviment mov;
	Posicio posOrigen(origen); // Utilitza constructor de Posicio amb string 
	Posicio posDesti(desti);

	mov.afegirPosicio(posOrigen);
	mov.afegirPosicio(posDesti);

	NodeMoviment* nouNode = new NodeMoviment();
	nouNode->moviment = mov;
	nouNode->seguent = nullptr;

	if (esBuida())
		m_primer = nouNode;
	else
		m_ultim->seguent = nouNode;
	m_ultim = nouNode;
}

Moviment CuaMoviments::treuMoviment()
{
	if (esBuida()) return Moviment();

	NodeMoviment* nodeBorrar = m_primer;
	Moviment moviment = nodeBorrar->moviment;

	m_primer = m_primer->seguent;
	if (m_primer == nullptr) m_ultim = nullptr;

	delete nodeBorrar;
	return moviment;
}

void CuaMoviments::guardaMoviments(const string& nomFitxer) const
{
	ofstream fitxer(nomFitxer);
	if (fitxer.is_open()) {
		NodeMoviment* actual = m_primer; 
		while (actual != nullptr) {
			Posicio origen = actual->moviment.getPosicio(0);  
			Posicio desti = actual->moviment.getPosicio(1);  

			fitxer << origen << " " << desti << endl;  

			actual = actual->seguent;  
		}
		fitxer.close(); 
	} 
	else {
		cout << "Error al obrir el fitxer. " << endl; // No ho mostrem en el tauler 
	}
}
