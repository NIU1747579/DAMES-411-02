#include "tauler.hpp"

// Inicializa el tablero del juego a partir de un fichero
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
    while (fitxer >> tipusFitxa >> pos) //leemos la pos y el valor
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

// Verifica si un movimiento de una ficha en un tablero de juego es valido. Este metodo toma las coordenadas
// de la posicion origen y destino, y un parametro de referencia esCaptura que indica si el movimiento
// implica capturar una ficha contraria
bool Tauler::esMovimentValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, bool& esCaptura)
{
    esCaptura = false;
    // Verificamos si la posicion de destino esta dentro de los limites
    if (filaDesti < 0 || filaDesti >= N_FILES || colDesti < 0 || colDesti >= N_COLUMNES) return false;

    const Fitxa& fitxaOrigen = m_tauler[filaOrigen][colOrigen];
    const Fitxa& fitxaDesti = m_tauler[filaDesti][colDesti];

    if (fitxaDesti.esBuida()) {
        if (fitxaOrigen.getTipus() == TIPUS_NORMAL) {
            // Calculamos la diferencia entre filas y columnas para determinar en que direccion se mueve la ficha
            int deltaFila = filaDesti - filaOrigen;
            int deltaCol = colDesti - colOrigen;

            bool direccioCorrecta;
            if (fitxaOrigen.getColor() == COLOR_BLANC) {
                direccioCorrecta = (deltaFila < 0);
            }
            else {
                direccioCorrecta = (deltaFila > 0);
            }

            // Se verifica si el movimiento es una simple diagonal de una casilla
            if (abs(deltaFila) == 1 && abs(deltaCol) == 1)
                return direccioCorrecta;

            // Se verifica si el movimiento es una captura (diagonal de dos o mas casillas) y si hay fichas contrarias
            // en el camino
            if (abs(deltaFila) >= 2 && abs(deltaCol) >= 2 &&
                abs(deltaFila) == abs(deltaCol)) {

                int dirFila = 1;
                if (deltaFila < 0) dirFila = -1;

                int dirCol = 1;
                if (deltaCol < 0) dirCol = -1;

                int fitxesContraries = 0;
                int fila = filaOrigen + dirFila;
                int col = colOrigen + dirCol;

                while (fila != filaDesti && col != colDesti) {
                    if (!m_tauler[fila][col].esBuida()) {
                        fitxesContraries++;
                        if (m_tauler[fila][col].getColor() == fitxaOrigen.getColor()) return false;
                    }
                    fila += dirFila;
                    col += dirCol;
                }

                if (fitxesContraries == abs(deltaFila) / 2) {
                    esCaptura = true;
                    return true;
                }
            }
            return false;
        }
        else if (fitxaOrigen.getTipus() == TIPUS_DAMA) {
            int deltaFila = filaDesti - filaOrigen;
            int deltaCol = colDesti - colOrigen;

            int dirFila = 1;
            if (deltaFila < 0) dirFila = -1;

            int dirCol = 1;
            if (deltaCol < 0) dirCol = -1;

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
    }
    return false;
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

                    Posicio posicionsPossibles[100];
                    int numPosicions = 0;
                    getPosicionsPossibles(posicioActual, numPosicions, posicionsPossibles);

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
                Posicio pos = Posicio(i, j);
                string x = pos.toString();

                fitxer << tauler[i][j] << " " << x << endl;
            }
        }
    }
    fitxer.close();
}

bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) {
    if (!origen.esValida() || !desti.esValida()) {
        return false;
    }

    Fitxa& fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
    if (fitxaOrigen.esBuida()) {
        return false;
    }

    bool esCaptura = false;
    if (!esMovimentValid(origen.getFila(), origen.getColumna(),
        desti.getFila(), desti.getColumna(), esCaptura)) {
        return false;
    }

    if (esCaptura) {
        int deltaFila = desti.getFila() - origen.getFila();
        int deltaCol = desti.getColumna() - origen.getColumna();

        int dirFila = (deltaFila > 0) ? 1 : -1;
        int dirCol = (deltaCol > 0) ? 1 : -1;

        int fila = origen.getFila() + dirFila;
        int col = origen.getColumna() + dirCol;

        while (fila != desti.getFila() && col != desti.getColumna()) {
            if (!m_tauler[fila][col].esBuida()) {
                m_tauler[fila][col].setTipus(TIPUS_EMPTY);
            }
            fila += dirFila;
            col += dirCol;
        }
    }

    // Mover la ficha
    m_tauler[desti.getFila()][desti.getColumna()] = fitxaOrigen;
    m_tauler[origen.getFila()][origen.getColumna()].setTipus(TIPUS_EMPTY);

    // Promocion a dama
    bool esBlanca = (fitxaOrigen.getColor() == COLOR_BLANC);
    bool arribaFinal = (esBlanca && desti.getFila() == 0) || (!esBlanca && desti.getFila() == N_FILES - 1);
    if (arribaFinal) {
        m_tauler[desti.getFila()][desti.getColumna()].setTipus(TIPUS_DAMA);
    }

    // Regla de "bufar" (eliminar ficha si no se capturo pudiendo hacerlo)
    if (!esCaptura) {
        bool hihaAltresCaptures = false;

        // Buscar en todo el tablero
        for (int i = 0; i < N_FILES; i++) {
            for (int j = 0; j < N_COLUMNES; j++) {
                if (!m_tauler[i][j].esBuida() &&
                    m_tauler[i][j].getColor() == fitxaOrigen.getColor()) {

                    Posicio pos(i, j);
                    Posicio posicionesPosibles[32];
                    int numPosiciones = 0;
                    getPosicionsPossibles(pos, numPosiciones, posicionesPosibles);

                    for (int k = 0; k < numPosiciones; k++) {
                        bool esUnaCaptura = false;
                        if (esMovimentValid(i, j, posicionesPosibles[k].getFila(),
                            posicionesPosibles[k].getColumna(), esUnaCaptura)) {
                            if (esUnaCaptura) {
                                hihaAltresCaptures = true;
                                // Eliminar la primera ficha que podria haber capturado
                                m_tauler[i][j].setTipus(TIPUS_EMPTY);
                                actualitzaMovimentsValids();
                                return true;
                            }
                        }
                    }
                }
            }
        }
    }

    actualitzaMovimentsValids();
    return true;
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

void Tauler::getPosicionsPossibles(const Posicio& origen, int& nPosicions, Posicio posicionsPossibles[]) {
    nPosicions = 0;
    if (!origen.esValida() || m_tauler[origen.getFila()][origen.getColumna()].esBuida()) {
        return;
    }

    const Fitxa& fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
    const int direcciones[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };
    bool hiHaCaptures = false;

    // Verificar capturas primero
    for (int d = 0; d < 4; d++) {
        int dirFila = direcciones[d][0];
        int dirCol = direcciones[d][1];

        if (fitxaOrigen.getTipus() == TIPUS_NORMAL) {
            // Para fichas normales
            int filaIntermedia = origen.getFila() + dirFila;
            int colIntermedia = origen.getColumna() + dirCol;
            int filaDesti = origen.getFila() + 2 * dirFila;
            int colDesti = origen.getColumna() + 2 * dirCol;

            if (filaIntermedia >= 0 && filaIntermedia < N_FILES &&
                colIntermedia >= 0 && colIntermedia < N_COLUMNES &&
                filaDesti >= 0 && filaDesti < N_FILES &&
                colDesti >= 0 && colDesti < N_COLUMNES) {

                if (!m_tauler[filaIntermedia][colIntermedia].esBuida() &&
                    m_tauler[filaIntermedia][colIntermedia].getColor() != fitxaOrigen.getColor() &&
                    m_tauler[filaDesti][colDesti].esBuida()) {

                    posicionsPossibles[nPosicions++] = Posicio(filaDesti, colDesti);
                    hiHaCaptures = true;
                }
            }
        }
        else {
            // Para damas
            int f = origen.getFila() + dirFila;
            int c = origen.getColumna() + dirCol;
            bool fichaContrariaEncontrada = false;

            while (f >= 0 && f < N_FILES && c >= 0 && c < N_COLUMNES) {
                if (!m_tauler[f][c].esBuida()) {
                    if (m_tauler[f][c].getColor() == fitxaOrigen.getColor()) {
                        break; // Ficha del mismo color
                    }
                    else if (!fichaContrariaEncontrada) {
                        fichaContrariaEncontrada = true;
                    }
                    else {
                        break; // Mas de una ficha contraria
                    }
                }
                else {
                    if (fichaContrariaEncontrada) {
                        posicionsPossibles[nPosicions++] = Posicio(f, c);
                        hiHaCaptures = true;
                    }
                }
                f += dirFila;
                c += dirCol;
            }
        }
    }

    // Si no hay capturas, verificar movimientos simples
    if (!hiHaCaptures) {
        if (fitxaOrigen.getTipus() == TIPUS_NORMAL) {
            int direccionFila = (fitxaOrigen.getColor() == COLOR_BLANC) ? -1 : 1;

            // Diagonal izquierda
            int novaFila = origen.getFila() + direccionFila;
            int novaCol = origen.getColumna() - 1;
            if (novaFila >= 0 && novaFila < N_FILES && novaCol >= 0 && novaCol < N_COLUMNES &&
                m_tauler[novaFila][novaCol].esBuida()) {
                posicionsPossibles[nPosicions++] = Posicio(novaFila, novaCol);
            }

            // Diagonal derecha
            novaCol = origen.getColumna() + 1;
            if (novaFila >= 0 && novaFila < N_FILES && novaCol >= 0 && novaCol < N_COLUMNES &&
                m_tauler[novaFila][novaCol].esBuida()) {
                posicionsPossibles[nPosicions++] = Posicio(novaFila, novaCol);
            }
        }
        else {
            // Para damas, movimientos simples en todas las direcciones
            for (int d = 0; d < 4; d++) {
                int dirFila = direcciones[d][0];
                int dirCol = direcciones[d][1];
                int f = origen.getFila() + dirFila;
                int c = origen.getColumna() + dirCol;

                while (f >= 0 && f < N_FILES && c >= 0 && c < N_COLUMNES && m_tauler[f][c].esBuida()) {
                    posicionsPossibles[nPosicions++] = Posicio(f, c);
                    f += dirFila;
                    c += dirCol;
                }
            }
        }
    }
}
