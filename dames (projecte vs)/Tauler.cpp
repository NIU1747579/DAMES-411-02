#include "tauler.hpp"

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

void Tauler::canviem(Posicio origen, Posicio& desti) {
	Fitxa& fitxa = m_tauler[origen.getFila()][origen.getColumna()];
	int deltaFila = desti.getFila() - origen.getFila();
	int deltaCol = desti.getColumna() - origen.getColumna();
	int dirFila;
	int dirCol;

	if (deltaFila > 0) {
		dirFila = 1;
	}
	else {
		dirFila = -1;
	}

	if (deltaCol > 0) {
		dirCol = 1;
	}
	else {
		dirCol = -1;
	}

	if (fitxa.getTipus() == TIPUS_NORMAL) {
		int filaIntermedia = origen.getFila() + dirFila;
		int colIntermedia = origen.getColumna() + dirCol;

		if (filaIntermedia >= 0 && filaIntermedia < N_FILES &&
			colIntermedia >= 0 && colIntermedia < N_COLUMNES &&
			!m_tauler[filaIntermedia][colIntermedia].esBuida() &&
			m_tauler[filaIntermedia][colIntermedia].getColor() != fitxa.getColor()) {

			int nuevaFila = origen.getFila() + 2 * dirFila;
			int nuevaCol = origen.getColumna() + 2 * dirCol;

			if (nuevaFila >= 0 && nuevaFila < N_FILES &&
				nuevaCol >= 0 && nuevaCol < N_COLUMNES &&
				m_tauler[nuevaFila][nuevaCol].esBuida()) {
				desti = Posicio(nuevaFila, nuevaCol);
				return;
			}
		}
	}
	else {
		int f = origen.getFila();
		int c = origen.getColumna();
		bool encontradoContrario = false;
		int filaContrario = -1;
		int colContrario = -1;

		while (true) {
			f += dirFila;
			c += dirCol;

			if (f < 0 || f >= N_FILES || c < 0 || c >= N_COLUMNES) {
				break;
			}

			if (!m_tauler[f][c].esBuida()) {
				if (m_tauler[f][c].getColor() != fitxa.getColor() && !encontradoContrario) {
					encontradoContrario = true;
					filaContrario = f;
					colContrario = c;
				}
				else {
					break;
				}
			}
			else if (encontradoContrario) {
				desti = Posicio(f, c);
				return;
			}
		}
	}

	desti = Posicio(origen.getFila() + deltaFila, origen.getColumna() + deltaCol);
}

bool Tauler::esMovimentValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, bool& esCaptura) {
	esCaptura = false;

	if (filaDesti < 0 || filaDesti >= N_FILES || colDesti < 0 || colDesti >= N_COLUMNES)
		return false;

	const Fitxa& fitxaOrigen = m_tauler[filaOrigen][colOrigen];
	const Fitxa& fitxaDesti = m_tauler[filaDesti][colDesti];

	if (fitxaDesti.esBuida()) {
		if (fitxaOrigen.getTipus() == TIPUS_NORMAL) {
			int deltaFila = filaDesti - filaOrigen;
			int deltaCol = colDesti - colOrigen;

			if ((fitxaOrigen.getColor() == COLOR_BLANC && deltaFila > 0) ||
				(fitxaOrigen.getColor() == COLOR_NEGRE && deltaFila < 0))
				return false;

			if (abs(deltaFila) == 2 && abs(deltaCol) == 2) {
				int filaCaptura = filaOrigen + (deltaFila / 2);
				int colCaptura = colOrigen + (deltaCol / 2);

				if (!m_tauler[filaCaptura][colCaptura].esBuida() &&
					m_tauler[filaCaptura][colCaptura].getColor() != fitxaOrigen.getColor()) {
					esCaptura = true;
					return true;
				}
				return false;
			}

			return (abs(deltaFila) == 1 && abs(deltaCol) == 1);
		}
		else if (fitxaOrigen.getTipus() == TIPUS_DAMA) {
			int deltaFila = filaDesti - filaOrigen;
			int deltaCol = colDesti - colOrigen;

			if (abs(deltaFila) != abs(deltaCol))
				return false;

			int dirFila;
			int dirCol;

			if (deltaFila > 0) {
				dirFila = 1;
			}
			else {
				dirFila = -1;
			}

			if (deltaCol > 0) {
				dirCol = 1;
			}
			else {
				dirCol = -1;
			}

			int f = filaOrigen + dirFila;
			int c = colOrigen + dirCol;
			bool fichaEncontrada = false;

			while (f != filaDesti && c != colDesti) {
				if (!m_tauler[f][c].esBuida()) {
					if (fichaEncontrada)
						return false;

					if (m_tauler[f][c].getColor() == fitxaOrigen.getColor())
						return false;

					fichaEncontrada = true;
					esCaptura = true;
				}
				f += dirFila;
				c += dirCol;
			}

			return true;
		}

		return true;
	}

	return false;
}

void Tauler::obtenirPosicionsPossibles(int fila, int col, Posicio posicions[], int& numPosicions) {
	numPosicions = 0;
	Fitxa fitxa = m_tauler[fila][col];

	if (fitxa.getTipus() == TIPUS_NORMAL) {
		int direccionFila;

		if (fitxa.getColor() == COLOR_BLANC) {
			direccionFila = -1;
		}
		else {
			direccionFila = 1;
		}

		if (fila + direccionFila >= 0 && fila + direccionFila < N_FILES) {
			if (col - 1 >= 0) {
				posicions[numPosicions++] = Posicio(fila + direccionFila, col - 1);
			}
			if (col + 1 < N_COLUMNES) {
				posicions[numPosicions++] = Posicio(fila + direccionFila, col + 1);
			}
		}

		if (fila + 2 * direccionFila >= 0 && fila + 2 * direccionFila < N_FILES && col - 2 >= 0) {
			if (!m_tauler[fila + direccionFila][col - 1].esBuida() &&
				m_tauler[fila + direccionFila][col - 1].getColor() != fitxa.getColor() &&
				m_tauler[fila + 2 * direccionFila][col - 2].esBuida()) {
				posicions[numPosicions++] = Posicio(fila + 2 * direccionFila, col - 2);
			}
		}

		if (fila + 2 * direccionFila >= 0 && fila + 2 * direccionFila < N_FILES && col + 2 < N_COLUMNES) {
			if (!m_tauler[fila + direccionFila][col + 1].esBuida() &&
				m_tauler[fila + direccionFila][col + 1].getColor() != fitxa.getColor() &&
				m_tauler[fila + 2 * direccionFila][col + 2].esBuida()) {
				posicions[numPosicions++] = Posicio(fila + 2 * direccionFila, col + 2);
			}
		}
	}
	else if (fitxa.getTipus() == TIPUS_DAMA) {
		const int direcciones[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

		for (int d = 0; d < 4; d++) {
			int dirFila = direcciones[d][0];
			int dirCol = direcciones[d][1];
			int f = fila;
			int c = col;
			bool fichaEncontrada = false;

			while (true) {
				f += dirFila;
				c += dirCol;

				if (f < 0 || f >= N_FILES || c < 0 || c >= N_COLUMNES) {
					break;
				}

				if (m_tauler[f][c].esBuida()) {
					posicions[numPosicions++] = Posicio(f, c);

					if (fichaEncontrada) {
						break;
					}
				}
				else {
					if (m_tauler[f][c].getColor() != fitxa.getColor() && !fichaEncontrada) {
						fichaEncontrada = true;
					}
					else {
						break;
					}
				}
			}
		}
	}
}

void Tauler::actualitzaMovimentsValids() {
	for (int fila = 0; fila < N_FILES; fila++) {
		for (int col = 0; col < N_COLUMNES; col++) {
			Fitxa& fitxaActual = m_tauler[fila][col];

			if (!fitxaActual.esBuida()) {
				fitxaActual.netejaMovimentsValids();

				Moviment movimentsValids[100];
				int numMovimentsValids = 0;

				Moviment movimentsPendents[100];
				int numMovimentsPendents = 0;

				Moviment movimentInicial;
				movimentInicial.afegirPosicio(Posicio(fila, col));
				movimentsPendents[numMovimentsPendents++] = movimentInicial;

				bool hiHaCaptures = false;
				bool posicionsVisitades[N_FILES][N_COLUMNES] = { false };

				while (numMovimentsPendents > 0) {
					Moviment movimentActual = movimentsPendents[--numMovimentsPendents];
					Posicio posicioActual = movimentActual.getUltimaPosicio();

					if (posicionsVisitades[posicioActual.getFila()][posicioActual.getColumna()]) {
						continue;
					}
					posicionsVisitades[posicioActual.getFila()][posicioActual.getColumna()] = true;

					Posicio posicionsPossibles[4];
					int numPosicions = 0;
					obtenirPosicionsPossibles(posicioActual.getFila(), posicioActual.getColumna(),posicionsPossibles, numPosicions);

					bool haAfegitMoviment = false;

					for (int i = 0; i < numPosicions; i++) {
						Posicio posDesti = posicionsPossibles[i];
						bool esCaptura = false;

						if (esMovimentValid(posicioActual.getFila(), posicioActual.getColumna(),
							posDesti.getFila(), posDesti.getColumna(), esCaptura)) {

							if (esCaptura) {
								hiHaCaptures = true;
								int filaCaptura = (posicioActual.getFila() + posDesti.getFila()) / 2;
								int colCaptura = (posicioActual.getColumna() + posDesti.getColumna()) / 2;

								Moviment nouMoviment = movimentActual;
								nouMoviment.afegirPosicio(posDesti);
								nouMoviment.afegirCaptura(Posicio(filaCaptura, colCaptura));

								if (numMovimentsPendents < 100) {
									movimentsPendents[numMovimentsPendents++] = nouMoviment;
								}
								haAfegitMoviment = true;
							}
							else if (!hiHaCaptures) {
								Moviment nouMoviment = movimentActual;
								nouMoviment.afegirPosicio(posDesti);
								if (numMovimentsValids < 100) {
									movimentsValids[numMovimentsValids++] = nouMoviment;
								}
							}
						}
					}

					if (!haAfegitMoviment && movimentActual.getNumPosicions() > 1 && hiHaCaptures) {
						if (numMovimentsValids < 100) {
							movimentsValids[numMovimentsValids++] = movimentActual;
						}
					}
				}

				for (int i = 0; i < numMovimentsValids; i++) {
					fitxaActual.afegeixMovimentValid(movimentsValids[i]);
				}
			}
		}
	}
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
	bool canvi;
	if (!origen.esValida() || !desti.esValida()) return false;

	Fitxa fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
	if (fitxaOrigen.esBuida()) return false;

	// Comprovem su el moviment es valid
	if (!esMovimentValid(origen.getFila(), origen.getColumna(), desti.getFila(), desti.getColumna(),canvi))
		return false;

	// Guardem si hi ha hagut captura
	bool esCaptura = (abs(desti.getFila() - origen.getFila()) == 2);
	Posicio posCaptura;

	// Eliminem la fitxa capturada si nhi ha
	if (esCaptura)
	{
		posCaptura = Posicio((origen.getFila() + desti.getFila()) / 2,
			(origen.getColumna() + desti.getColumna()) / 2);

		// Verifiquem que realment hi ha fitxa contraria a capturar
		Fitxa fitxaCaptura = m_tauler[posCaptura.getFila()][posCaptura.getColumna()];
		if (fitxaCaptura.esBuida() || fitxaCaptura.getColor() == fitxaOrigen.getColor())
		{
			return false;
		}
		m_tauler[posCaptura.getFila()][posCaptura.getColumna()].setTipus(TIPUS_EMPTY);
	}

	// Movem la fitxa
	m_tauler[desti.getFila()][desti.getColumna()] = fitxaOrigen;
	m_tauler[origen.getFila()][origen.getColumna()].setTipus(TIPUS_EMPTY);

	// Convertim a dama si cal
	if ((fitxaOrigen.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
		(fitxaOrigen.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1))
	{
		m_tauler[desti.getFila()][desti.getColumna()].setTipus(TIPUS_DAMA);
	}

	// Comprovem si cal bufar la nostra fitxa
	if (!esCaptura)
	{
		bool hihaAltresCaptures = false;
		int i = 0;

		// Busquem en el tauler si hi ha cap possibles
		while (i < N_FILES && !hihaAltresCaptures)
		{
			int j = 0;
			while (j < N_COLUMNES && !hihaAltresCaptures)
			{
				if (!m_tauler[i][j].esBuida() && m_tauler[i][j].getColor() == fitxaOrigen.getColor())
				{
					int k = 0;
					while (k < m_tauler[i][j].getNumMoviments() && !hihaAltresCaptures)
					{
						if (m_tauler[i][j].getMoviment(k).getNCaptures() > 0)
						{
							hihaAltresCaptures = true;
						}
						k++;
					}
				}
				j++;
			}
			i++;
		}

		if (hihaAltresCaptures)
		{
			// Bufar una fitxa propia
			bool fitxaBorrada = false;
			for (int x = 0; x < N_FILES && !fitxaBorrada; x++)
			{
				for (int y = 0; y < N_COLUMNES && !fitxaBorrada; y++)
				{
					if (!m_tauler[x][y].esBuida() && m_tauler[x][y].getColor() == fitxaOrigen.getColor())
					{
						m_tauler[x][y].setTipus(TIPUS_EMPTY);
						fitxaBorrada = true;
					}
				}
			}
		}
	}
	actualitzaMovimentsValids();
	return true;
}

void Tauler::bufarFitxa(int fila, int columna) //elimina fitxa del jugador
{
	//fem una comprovacio per asegurarnos que no surt dels limitis
	if (fila >= 0 && fila < N_FILES && columna >= 0 && columna < N_COLUMNES)
		m_tauler[fila][columna].setTipus(TIPUS_EMPTY);

}

string Tauler::toString() const
{
	string resultat;
	string files = "87654321";

	for (int i = 0; i < N_FILES; i++)
	{
		resultat += files[i];
		resultat += ": ";
		for (int j = 0; j < N_COLUMNES; j++)
		{
			if (m_tauler[i][j].esBuida()) {
				resultat += "_ ";
			}
			else {
				if (m_tauler[i][j].getColor() == COLOR_NEGRE)
				{
					if (m_tauler[i][j].getTipus() == TIPUS_NORMAL) {
						resultat += "X ";
					}
					else {
						resultat += "R ";
					}
				}

				else {
					if (m_tauler[i][j].getColor() == COLOR_BLANC) {

						if (m_tauler[i][j].getTipus() == TIPUS_NORMAL) {
							resultat += "O ";
						}
						else {
							resultat += "D ";
						}
					}
				}
			}
		}
		resultat += "\n";
	}
	resultat += "   a b c d e f g h";
	return resultat;
}



void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
	bool canvi;
	nPosicions = 0;
	bool possible = true;
	if (!origen.esValida() || m_tauler[origen.getFila()][origen.getColumna()].esBuida()) {
		possible = false;
	}

	Fitxa fitxa = m_tauler[origen.getFila()][origen.getColumna()];
	Posicio posicionsTemporals[100];
	int numPosicionsTemporals = 0;

	// Obtenim totes les posicions possibles inicials
	obtenirPosicionsPossibles(origen.getFila(), origen.getColumna(),
		posicionsTemporals, numPosicionsTemporals);

	// Busquem les posicions valides per a la fitxa
	for (int i = 0; i < numPosicionsTemporals; i++) {
		Posicio desti = posicionsTemporals[i];

		if (esMovimentValid(origen.getFila(), origen.getColumna(),
			desti.getFila(), desti.getColumna(),canvi)) {

			// Si son mov de captura, verifiquem si son oblig
			if (abs(desti.getFila() - origen.getFila()) == 2) {
				//  Si es mov de captura
				posicionsPossibles[nPosicions++] = desti;
			}
			else {
				// Moviment simple,  nomes lafegim si no hi ha cap
				if (numPosicionsTemporals == 2) {
					posicionsPossibles[nPosicions++] = desti;
				}
			}
		}
	}

	// Si hi ha captures disponibles eliminem els moviments simp
	if (nPosicions > 2) { // Si hi ha moviments de captura
		int numCaptures = 0;
		for (int i = 0; i < nPosicions; i++) {
			if (abs(posicionsPossibles[i].getFila() - origen.getFila()) == 2) {
				posicionsPossibles[numCaptures++] = posicionsPossibles[i];
			}
		}
		nPosicions = numCaptures;
	}
}
