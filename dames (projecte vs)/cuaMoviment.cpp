#include "cuaMoviment.h"

void CuaMoviments::neteja() 
{
	while (!esBuida()) treuMoviment();  
}
void CuaMoviments::afegeixMoviment(const string& origen, const string& desti) 
{
	Moviment mov; 
	Posicio posOrigen(origen); // Utilitza constructor de Posicio amb string 
	Posicio posDesti(desti); 

	mov.afegirPosicio(posOrigen); 
	mov.afegirPosicio(posDesti); 

	NodeMoviment* nouNode = new NodeMoviment; 
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
			for (int i = 0; i < actual->moviment.getNumPosicions(); ++i) {
				Posicio pos = actual->moviment.getPosicio(i);
				fitxer << pos.getFila() << " " << pos.getColumna() << " ";
			}
			fitxer << "\n";
			actual = actual->seguent;
		}
	}
}

void CuaMoviments::carregaMoviments(const string& nomFitxer) 
{
	neteja();
	ifstream fitxer(nomFitxer);
	if (fitxer.is_open()) {
		string origen, desti;
		while (fitxer >> origen >> desti) {
			afegeixMoviment(origen, desti);
		}
	}
	fitxer.close();
}
