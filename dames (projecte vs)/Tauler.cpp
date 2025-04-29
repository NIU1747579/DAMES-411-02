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

				while (numMovimentsPendents > 0) {
					Moviment movimentActual = movimentsPendents[--numMovimentsPendents];
					Posicio posicioActual = movimentActual.getUltimaPosicio();

					Posicio posicionsPossibles[4];
					int numPosicions = 0;
					obtenirPosicionsPossibles(posicioActual.getFila(), posicioActual.getColumna(),
						posicionsPossibles, numPosicions);

					bool haAfegitMoviment = false;

					for (int i = 0; i < numPosicions; i++) {
						Posicio posDesti = posicionsPossibles[i];

						if (esMovimentValid(posicioActual.getFila(), posicioActual.getColumna(),
							posDesti.getFila(), posDesti.getColumna())) {

							Moviment nouMoviment = movimentActual;
							nouMoviment.afegirPosicio(posDesti);

							if (abs(posDesti.getFila() - posicioActual.getFila()) == 2) {
								// Calcular posición de la ficha capturada
								int filaCaptura = (posicioActual.getFila() + posDesti.getFila()) / 2;
								int colCaptura = (posicioActual.getColumna() + posDesti.getColumna()) / 2;
								nouMoviment.afegirCaptura(Posicio(filaCaptura, colCaptura));

								if (numMovimentsPendents < 100) {
									movimentsPendents[numMovimentsPendents++] = nouMoviment;
								}
								haAfegitMoviment = true;
							}
							else {
								if (numMovimentsValids < 100) {
									movimentsValids[numMovimentsValids++] = nouMoviment;
								}
							}
						}
					}

					if (!haAfegitMoviment && movimentActual.getNumPosicions() > 1) {
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

void Tauler::obtenirPosicionsPossibles(int fila, int col, Posicio posicions[], int& numPosicions) {
	numPosicions = 0;
	Fitxa fitxa = m_tauler[fila][col];

	if (fitxa.getTipus() == TIPUS_NORMAL) {
		if (fitxa.getColor() == COLOR_BLANC) {
			if (fila < N_FILES - 1) {
				if (col > 0) {
					posicions[numPosicions++] = Posicio(fila + 1, col - 1); 
				}
				if (col < N_COLUMNES - 1) {
					posicions[numPosicions++] = Posicio(fila + 1, col + 1);
				}
			}
		}
		else {
			if (fila > 0) {
				if (col > 0) {
					posicions[numPosicions++] = Posicio(fila - 1, col - 1);
				}
				if (col < N_COLUMNES - 1) {
					posicions[numPosicions++] = Posicio(fila - 1, col + 1);
				}
			}
		}
	}
	else if (fitxa.getTipus() == TIPUS_DAMA) {
		const int direccions[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

		for (int d = 0; d < 4; d++) {
			int df = direccions[d][0];
			int dc = direccions[d][1];

			for (int i = 1; i < N_FILES; i++) {
				int nuevaFila = fila + i * df;
				int nuevaCol = col + i * dc;

				if (nuevaFila >= 0 && nuevaFila < N_FILES &&
					nuevaCol >= 0 && nuevaCol < N_COLUMNES) {
					posicions[numPosicions++] = Posicio(nuevaFila, nuevaCol);
				}
				else {
					break; // Salir del bucle si salimos del tablero
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

bool Tauler::esMovimentValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti)
{

	if (filaDesti < 0 || filaDesti >= N_FILES || colDesti < 0 || colDesti >= N_COLUMNES)
		return false;

	if (m_tauler[filaDesti][colDesti].getTipus() == TIPUS_EMPTY)
		return true;

	//aqui miramos que si hay una fitxa de color contrario, podamos matar
	Fitxa fitxaOrigen = m_tauler[filaOrigen][colOrigen];
	Fitxa fitxaDesti = m_tauler[filaDesti][colDesti];

	if (fitxaDesti.getColor() != fitxaOrigen.getColor()) {
		int filaCaptura = filaDesti + (filaDesti - filaOrigen);
		int colCaptura = colDesti + (colDesti - colOrigen);

		if (filaCaptura >= 0 && filaCaptura < N_FILES && colCaptura >= 0 && colCaptura < N_COLUMNES)
			return m_tauler[filaCaptura][colCaptura].getTipus() == TIPUS_EMPTY;
	}

	return false;
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti)
{
	if (!origen.esValida() || !desti.esValida()) return false;

	Fitxa fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
	if (fitxaOrigen.esBuida()) return false;

	// Comprovem su el moviment es valid
	if (!esMovimentValid(origen.getFila(), origen.getColumna(), desti.getFila(), desti.getColumna()))
		return false;

	// Guardem si hi ha hagut captura
	bool esCaptura = (abs(desti.getFila() - origen.getFila()) == 2);
	Posicio posCaptura;

	if (esCaptura)
	{
		posCaptura = Posicio((origen.getFila() + desti.getFila() / 2),
			(origen.getColumna() + desti.getColumna()) / 2);
	}

	// Verifiquem que realment hi ha fitxa contraria a capturar
	Fitxa fitxaCaptura = m_tauler[posCaptura.getFila()][posCaptura.getColumna()];

	if (fitxaCaptura.esBuida() || fitxaCaptura.getColor() == fitxaOrigen.getColor())
	{
		return false;
	}

	// Movem la fitxa
	m_tauler[desti.getFila()][desti.getColumna()] = fitxaOrigen;
	m_tauler[origen.getFila()][origen.getColumna()].setTipus(TIPUS_EMPTY);

	// Eliminem la fitxa capturada si nhi ha
	if (esCaptura)
	{
		m_tauler[posCaptura.getFila()][posCaptura.getColumna()].setTipus(TIPUS_EMPTY);
	}

	// Convertim a dama si cal
	if ((fitxaOrigen.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
		(fitxaOrigen.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1))
	{
		m_tauler[desti.getFila()][desti.getColumna()].setTipus(TIPUS_DAMA);
	}

	// Comprovem si cal bufar la nostra fitxa
	if (!esCaptura)
	{
		// Comprovem si hi havia mov de captura
		bool hihaAltresCaptures = false;
		Moviment moviments = fitxaOrigen.getMoviments();

		for (int i = 0; i < moviments.getNMoviments(); i++)
		{
			// Si trobem un mov amb captures, actualitzem la variable
			if (moviments.getMoviment(i).getNCaptures() > 0)
			{
				hihaAltresCaptures = true;
				break; //CANVIARHOOOOOOOOOO
			}
		}

		if (hihaAltresCaptures)
		{
			// bufar una fitxa propia
			for (int i = 0; i < N_FILES; i++)
			{
				for (int j = 0; j < N_COLUMNES; j++)
				{
					if (!m_tauler[i][j].esBuida() && m_tauler[i][j].getColor() == fitxaOrigen.getColor())
					{
						m_tauler[i][j].setTipus(TIPUS_EMPTY);
						i = N_FILES; // Per sortir del bucle exter
						break;
					}
				}
			}
		}
		actualitzaMovimentsValids();
		return true;
	}
}
/*

int i = 0;
bool movimentTrobat = false;
//busquem si el desti esta entre els mov valids
while (i < mov.geNMoviments() && !movimentTrobat)
{
	movimentActual = mov.getMoviments(i);

	if (movimentActual.getDesti() == desti) movimentTrobat = true;
	else
		i++;
}

if (!movimentTrobat) return false;

//eliminar fitxes contraries - captures
if (movimentActual.getNCaptures() > 0)
{
	for (int i = 0; i < movimentActual.getNCaptures(); i++)
	{
		Posicio posCaptura = movimentActual.getCaptures(i);
		if (posCaptura.esValida())
		{
			bufarFitxa(posCaptura.getFila(), posCaptura.getColumna());
		}
	}
}
else {
	if (movimentActual.getNCaptures() == 0)
	{
		bufarFitxa(origen.getFila(), desti.getColumna()); //per a captures
	}
}

//mov fitxa
m_tauler[desti.getFila()][desti.getColumna()] = fitxaOrigen;
m_tauler[origen.getFila()][origen.getColumna()].setTipus(TIPUS_EMPTY);

//comprovar si la fitxa ha arribat al final per convertir a dama
if ((fitxaOrigen.getColor() == COLOR_BLANC && desti.getFila() == 0) ||
	(fitxaOrigen.getColor() == COLOR_NEGRE && desti.getFila() == N_FILES - 1))
{
	m_tauler[desti.getFila()][desti.getColumna()].setTipus(TIPUS_DAMA);
}

actualitzaMovimentsValids();
return true;
*/


void Tauler::bufarFitxa(int fila, int columna) //elimina fitxa del jugador
{
	//fem una comprovacio per asegurarnos que no surt dels limitis
	if (fila >= 0 && fila < N_FILES && columna >= 0 && columna < N_COLUMNES)
		m_tauler[fila][columna].setTipus(TIPUS_EMPTY);

}

string Tauler::ToString() const
{
	string resultat;
	int num = 8;

	for (int i = 0; i < N_FILES; i++)
	{
		resultat += num + ": ";
		for (int j = 0; j < N_COLUMNES; j++)
		{
			if (m_tauler[N_FILES][N_COLUMNES].getColor() == COLOR_NEGRE)
			{
				if (m_tauler[N_FILES][N_COLUMNES].getTipus() == TIPUS_NORMAL) {
					resultat += 'X ';
				}
				else
				{
					resultat += 'R ';
				}

			}

			else {
				if (m_tauler[N_FILES][N_COLUMNES].getColor() == COLOR_BLANC)
				{
					if (m_tauler[N_FILES][N_COLUMNES].getTipus() == TIPUS_NORMAL) {
						resultat += 'O ';
					}
					else
					{
						resultat += 'D ';
					}
				}

				else {
					resultat += '_ ';
				}
			}
		}
		num--;
		resultat += "\n";
	}
	resultat += "  A B C D E F G H";
	return resultat;
}

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[])
{
	nPosicions = 0;
	bool possible = true;
	if (!origen.esValida()) possible = false;
	if (m_tauler[origen.getFila()][origen.getColumna()].esBuida()) possible = false;

	Fitxa fitxa = m_tauler[origen.getFila()][origen.getColumna()];
	Moviment moviments = fitxa.getMoviments();

	// Afegir totes les posicions desti uniques
	for (int i = 0; i < moviments.getNMoviments(); i++)
	{
		Posicio desti = moviments.getMoviment(i).getDesti();

		// comprova si ja esta a la llista
		bool repetida = false;
		for (int j = 0; j < nPosicions && !repetida; j++)
		{
			if (posicionsPossibles[j] == desti)
				repetida = true;
		}
		if (!repetida)
		{
			posicionsPossibles[nPosicions] = desti;
			nPosicions++;
		}
	}
}


string Tauler::ToString() const
{
	string resultat;
	int num = 8;

	for (int i = 0; i < N_FILES; i++)
	{
		resultat += num + ": ";
		for (int j = 0; j < N_COLUMNES; j++)
		{
			if (m_tauler[N_FILES][N_COLUMNES].getColor() == COLOR_NEGRE)
			{
				if (m_tauler[N_FILES][N_COLUMNES].getTipus() == TIPUS_NORMAL) {
					resultat += 'X ';
				}
				else
				{
					resultat += 'R ';
				}
				
			}

			else {
				if (m_tauler[N_FILES][N_COLUMNES].getColor() == COLOR_BLANC)
				{
					if (m_tauler[N_FILES][N_COLUMNES].getTipus() == TIPUS_NORMAL) {
						resultat += 'O ';
					}
					else
					{
						resultat += 'D ';
					}
				}

				else {
					resultat += '_ ';
				}
			}
		}
		num--;
		resultat += "\n";
	}
	resultat += "  A B C D E F G H";
	return resultat;
}
