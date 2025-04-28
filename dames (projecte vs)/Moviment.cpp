#include "Moviment.h"

void Moviment::afegirCaptura(const Posicio& c)
{
	if (m_nCaptures < 12)
	{
		m_captures[m_nCaptures++] = c;
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

void Moviment::actualitzaMovimentsNN(int fila, int columna)
{
	m_movimentsValids.inicialitza();
}

void Moviment::actualitzaMovimentsND(int fila, int columna)
{

}

void Moviment::actualitzaMovimentsBN(int fila, int columna)
{

}

void Moviment::actualitzaMovimentsBD(int fila, int columna)
{

}

*/
