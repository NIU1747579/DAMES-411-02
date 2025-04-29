#include "Moviment.h"

void Moviment::afegirCaptura(const Posicio& c)
{
	if (m_numCaptures < 12)
	{
		m_captures[m_numCaptures++] = c;
	}
}

void Moviment::afegirPosicio(const Posicio& pos) {
	if (m_numPosicions < 20) {
		m_posicions[m_numPosicions] = pos;
		m_numPosicions++;
	}
}

Posicio Moviment::getUltimaPosicio() const {
	if (m_numPosicions > 0) {
		return m_posicions[m_numPosicions - 1];
	}
	return Posicio(); // Devuelve posición inválida si no hay posiciones
}
