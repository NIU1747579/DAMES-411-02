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

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
	//primer verifiquem que el mov es valid (abans de moure la fitxa) 
	if (!movimentValid(origen, desti))
	{
		return false;
	}
}

bool Tauler::movimentValid(const Posicio& origen, const Posicio desti) const
{
	//verifiquem que les posicion estan al tauler - son valides
	if (!origen.esValida() || !desti.esValida())
	{
		return false;
	}
	//verifiquem que hi ha fitxa a lorigen i despres que no hi ha al desti
	Fitxa fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
	if (fitxaOrigen.esBuida()) return false;
	Fitxa fitxaDesti = m_tauler[desti.getFila()][desti.getColumna()];
	if (!fitxaDesti.esBuida()) return false;

	Moviment mov;
	mov.setOrigen(origen);
	mov.setDesti(desti);

	//verifiquem si el moviment que volem dur a terme es diagonal o no
	if (!mov.esMovimentDiagonal()) return false;

	//fer TIPUS_NORMAL


	//fer TIPUS_DAMA

}

void Tauler::bufarFitxa(int fila, int columna) //elimina fitxa del jugador
{
	//fem una comprovacio per asegurarnos que no surt dels limitis
	if (fila >= 0 && fila < N_FILES && columna >= 0 && columna < N_COLUMNES)
	{
		m_tauler[fila][columna].setTipus(TIPUS_EMPTY);
	}
}