#include "Fitxa.h"

void Fitxa::netejaMovimentsValids() {
	for (int i = 0; i < m_numMoviments; i++) {
		m_movimentsValids[i] = Moviment();
	}
	m_numMoviments = 0;
}

void Fitxa::afegeixMovimentValid(const Moviment& moviment) {
	if (m_numMoviments < 20) {
		m_movimentsValids[m_numMoviments] = moviment;
		m_numMoviments++;
	}
}
