#include "Moviment.h"


Posicio Moviment::getUltimaPosicio() const {
    if (!m_posicions.empty()) 
        return m_posicions.back(); 
    
    return Posicio(); // Devuelve pos invalida si no hay posiciones
}

int Moviment::getNumPosicions() const {
    return (int)m_posicions.size();
}
