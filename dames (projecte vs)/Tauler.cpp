#include "Tauler.h"

void Tauler::inicialitza(const string& nomFitxer)
{
	ifstream fitxer(nomFitxer);

	for (int i = 0; i < N_FILES; i++)
	{
		for (int j = 0; j < N_COLUMNES; j++)
		{
			m_tauler[i][j].setTipus(TIPUS_EMPTY);
		}
	}

	char tipusFitxa;
	string pos;
	while (fitxer >> tipusFitxa >> pos) //llegim la pos i el valor
	{
		Posicio posicio;
		posicio.fromString(pos);
		switch (tipusFitxa)
		{

		case 'O':
			m_tauler[posicio.getFila()][posicio.getColumna()].setColor(COLOR_BLANC);
			m_tauler[posicio.getFila()][posicio.getColumna()].setTipus(TIPUS_NORMAL);
			break;
		case 'X':
			m_tauler[posicio.getFila()][posicio.getColumna()].setColor(COLOR_NEGRE);
			m_tauler[posicio.getFila()][posicio.getColumna()].setTipus(TIPUS_NORMAL);
			break;
		case 'D':
			m_tauler[posicio.getFila()][posicio.getColumna()].setColor(COLOR_BLANC);
			m_tauler[posicio.getFila()][posicio.getColumna()].setTipus(TIPUS_DAMA);
			break;
		case 'R':
			m_tauler[posicio.getFila()][posicio.getColumna()].setColor(COLOR_NEGRE);
			m_tauler[posicio.getFila()][posicio.getColumna()].setTipus(TIPUS_DAMA);
			break;
		}
	}
	fitxer.close();
}
void Tauler::actualitzaMovimentsValids() const
{
	comproVarDama();
	inicialitzamoviments();
	do {
		obtenirSeguentMovimentPendent();
		calcularPosicionsDesDeMoviment();

		//tenenr en uenta si has matado o no

	} while ()


}

/*
void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES])
{
	ofstream fitxer(nomFitxer);

	for (int i = 0; i < N_FILES; i++)
	{
		for (int j = 0; j < N_COLUMNES; j++)
		{
			if (tauler[i][j] != ' ')
			{
				Posicio pos = Posicio(i, j); //?
				string x = pos.toString();

				fitxer << tauler[i][j] << " " << x << endl;
			}
		}
	}
	fitxer.close();
}
*/