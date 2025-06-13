#include "Moviment.h"

Posicio Moviment::getUltimaPosicio() const {
    if (!m_posicions.empty()) 
        return m_posicions.back(); 
    
    return Posicio(); // Devuelve pos invalida si no hay posiciones
}

Moviment::Moviment(const string& origen, const string& desti)
	: m_origen(origen), m_desti(desti) {
	// Inicializa las posiciones con origen y destino
	m_posicions.push_back(Posicio(origen));
	m_posicions.push_back(Posicio(desti));
}
int Moviment::getNumPosicions() const {
    return (int)m_posicions.size();
}

Posicio Moviment::getPosicioOrigen() const {
    if (m_posicions.size() > 0)
        return m_posicions[0];
    return Posicio();
}

Posicio Moviment::getPosicioDesti() const {
    if (m_posicions.size() > 1)
        return m_posicions[1];
    return Posicio();
}
