#include "Moviment.h"

void Moviment::afegirCaptura(const Posicio& c)
{
	if (m_numCaptures < 12)
	{
		m_captures[m_numCaptures++] = c;
	}
}

/*bool Moviment::esMovimentDiagonal() const
{
	int res_fila = abs(m_desti.getFila() - m_origen.getFila());
	int res_columna = abs(m_desti.getColumna() - m_origen.getColumna());
	if (res_fila == res_columna)
		return true;
	else
		return false;
}

*/

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
