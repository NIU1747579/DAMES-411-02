#include "tauler.hpp"


Tauler::Tauler() {
    for (int i = 0; i < N_FILES; i++) {
        for (int j = 0; j < N_COLUMNES; j++) {
            m_tauler[i][j] = nullptr;
        }
    }
}

Tauler::~Tauler() {
    for (int i = 0; i < N_FILES; i++) {
        for (int j = 0; j < N_COLUMNES; j++) {
            delete m_tauler[i][j];
        }
    }
}

// Inicializa el tablero de juego desde un archivo
void Tauler::inicialitza(const string& nomFitxer)
{
    ifstream fitxer(nomFitxer);

    // Inicializa todas las posiciones del tablero como vacias
    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            // Lliberar memoria previa si existeix
            if (m_tauler[i][j] != nullptr) {
                delete m_tauler[i][j];
                m_tauler[i][j] = nullptr;
            }
        }
    }

    // Lee el contenido del archivo: tipo de ficha y posicion
    char tipusFitxa;
    string pos;
    while (fitxer >> tipusFitxa >> pos) // Lee posicion y valor
    {
        Posicio posicio;
        posicio.fromString(pos);
        assignaFitxa(tipusFitxa, posicio);
    }
    fitxer.close();
}

// Asigna ficha a posicion del tablero segun su tipo
void Tauler::assignaFitxa(char tipusFitxa, const Posicio& posicio) {
    int fila = posicio.getFila();
    int col = posicio.getColumna();

    if (m_tauler[fila][col] != nullptr) {
        delete m_tauler[fila][col];
        m_tauler[fila][col] = nullptr;
    }

    switch (tipusFitxa)
    {
    case 'O':  // Ficha normal blanca
        m_tauler[fila][col] = new Fitxa(TIPUS_NORMAL, COLOR_BLANC);
        break;
    case 'X':  // Ficha normal negra
        m_tauler[fila][col] = new Fitxa(TIPUS_NORMAL, COLOR_NEGRE);
        break;
    case 'D':  // Dama blanca
        m_tauler[fila][col] = new Fitxa(TIPUS_DAMA, COLOR_BLANC);
        break;
    case 'R':  // Dama negra
        m_tauler[fila][col] = new Fitxa(TIPUS_DAMA, COLOR_NEGRE);
        break;
    }
}

Fitxa* Tauler::getFitxa(int fila, int col) const
{
    if (fila < 0 || fila >= N_FILES || col < 0 || col >= N_COLUMNES) return nullptr;
    return m_tauler[fila][col];
}

void Tauler::visualitzaTauler(const Posicio& fitxaSeleccionada, const vector<Posicio>& movimentsValids) {
    for (int fila = 0; fila < NUM_FILES_TAULER; ++fila) {
        for (int col = 0; col < NUM_COLS_TAULER; ++col) {
            if (m_tauler[fila][col] != nullptr) {
                int x = POS_X_TAULER + CASELLA_INICIAL_X + (col * AMPLADA_CASELLA);
                int y = POS_Y_TAULER + CASELLA_INICIAL_Y + (fila * ALCADA_CASELLA);

                m_tauler[fila][col]->visualitzaFitxa(x, y);

            }
        }
    }
    // 2. Dibuixar les posicions valides (en verd)
    for (size_t i = 0; i < movimentsValids.size(); ++i) {
        const Posicio& pos = movimentsValids[i];
        int x = POS_X_TAULER + CASELLA_INICIAL_X + (pos.getColumna() * AMPLADA_CASELLA);
        int y = POS_Y_TAULER + CASELLA_INICIAL_Y + (pos.getFila() * ALCADA_CASELLA);

        // No dibuixar sobre la fitxa seleccionada
        if (!(pos == fitxaSeleccionada)) {
            GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, x, y);
        }
    }
}

void Tauler::visualitzaMovimentsValids(const vector<Posicio>& posicions) const {
    for (size_t i = 0; i < posicions.size(); i++) {
        const Posicio& posActual = posicions[i];
        if (posActual.esValida()) {
            int posX = POS_X_TAULER + CASELLA_INICIAL_X + (posActual.getColumna() * AMPLADA_CASELLA);
            int posY = POS_Y_TAULER + CASELLA_INICIAL_Y + (posActual.getFila() * ALCADA_CASELLA);
            GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, posX, posY);
        }
    }
}

bool Tauler::comprovarFiPartida(ColorFitxa& guanyador) const {
    bool blancTeFitxes = false;
    bool negreTeFitxes = false;

    for (int i = 0; i < NUM_FILES_TAULER; i++) {
        for (int j = 0; j < NUM_COLS_TAULER; j++) {
            if (m_tauler[i][j] != nullptr) {

                if (m_tauler[i][j]->getColor() == COLOR_BLANC) 
                    blancTeFitxes = true;
                else 
                    negreTeFitxes = true;
                
            }
        }
    }

    if (!blancTeFitxes) {
        guanyador = COLOR_NEGRE;
        return true;
    }

    if (!negreTeFitxes) {
        guanyador = COLOR_BLANC;
        return true;
    }

    return false;
}

// Escribe el estado del tablero en un archivo
void escriuTauler(const string& nomFitxer, char tauler[N_FILES][N_COLUMNES])
{
    ofstream fitxer(nomFitxer);

    for (int i = 0; i < N_FILES; i++)
    {
        for (int j = 0; j < N_COLUMNES; j++)
        {
            if (tauler[i][j] != ' ')  // Solo escribe posiciones no vacias
            {
                Posicio pos = Posicio(i, j);
                string x = pos.toString();

                fitxer << tauler[i][j] << " " << x << endl;
            }
        }
    }
    fitxer.close();
}

// Calcula direccion de movimiento segun diferencia de posicion
int Tauler::calculaDireccio(int diferencia) const {
    if (diferencia > 0) return 1;
    if (diferencia < 0) return -1;
    return 0;
}

// Comprueba si el destino esta dentro de los limites del tablero
bool Tauler::esDestiDinsLimits(int fila, int col)
{
    return fila >= 0 && fila < N_FILES && col >= 0 && col < N_COLUMNES;
}

// Valida si un movimiento es legal
bool Tauler::esMovimentValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, bool& esCaptura)
{
    esCaptura = false;
    if (!esDestiDinsLimits(filaDesti, colDesti)) return false;

    // Comprova que hi hagi fitxa
    if (m_tauler[filaOrigen][colOrigen] == nullptr || m_tauler[filaDesti][colDesti] != nullptr) {
        return false;
    }

    Fitxa* fitxaOrigen = m_tauler[filaOrigen][colOrigen];

    if (fitxaOrigen->getTipus() == TIPUS_NORMAL) {
        return movimentFitxaNormalValid(filaOrigen, colOrigen, filaDesti, colDesti, *fitxaOrigen, esCaptura);
    }
    else if (fitxaOrigen->getTipus() == TIPUS_DAMA) {
        return movimentDamaValid(filaOrigen, colOrigen, filaDesti, colDesti, *fitxaOrigen, esCaptura);
    }

    return false;
}

// Funcion para validar movimiento de ficha normal
// Modifica la función esMovimentValid para fichas normales
bool Tauler::movimentFitxaNormalValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, const Fitxa& fitxaOrigen, bool& esCaptura) {
    int deltaFila = filaDesti - filaOrigen;
    int deltaCol = colDesti - colOrigen;

    // Movimiento simple (1 casilla)
    if (abs(deltaFila) == 1 && abs(deltaCol) == 1) {
        // Verifica dirección correcta solo si no es dama
        if (fitxaOrigen.getTipus() == TIPUS_NORMAL) {
            bool direccioCorrecta;
            if (fitxaOrigen.getColor() == COLOR_BLANC) {
                direccioCorrecta = (deltaFila < 0);  // Blancas mueven hacia arriba
            }
            else {
                direccioCorrecta = (deltaFila > 0);  // Negras mueven hacia abajo
            }
            if (!direccioCorrecta) return false;
        }
        esCaptura = false;
        return true;
    }

    // Movimiento de captura (2+ casillas)
    if (abs(deltaFila) >= 2 && abs(deltaCol) >= 2 && abs(deltaFila) == abs(deltaCol)) {
        int dirFila = calculaDireccio(deltaFila);
        int dirCol = calculaDireccio(deltaCol);
        int fitxesContraries = 0;
        int fila = filaOrigen + dirFila;
        int col = colOrigen + dirCol;

        while (fila != filaDesti && col != colDesti) {
            if (m_tauler[fila][col] != nullptr) {
                if (m_tauler[fila][col]->getColor() == fitxaOrigen.getColor()) {
                    return false;
                }
                fitxesContraries++;
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

// Modifica getPosicionsPossibles para manejar kills múltiples
// En la función getPosicionsPossibles, cambia esto:
void Tauler::getPosicionsPossibles(const Posicio& origen, vector<Posicio>& posicionsPossibles) {
    posicionsPossibles.clear();
    if (!origen.esValida() || m_tauler[origen.getFila()][origen.getColumna()] == nullptr) 
        return;
    
    Fitxa* fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];

    // Eliminamos la comprobación de capturas obligatorias
    if (fitxaOrigen->getTipus() == TIPUS_NORMAL) {
        const int direccionesBlancas[2][2] = { {-1,1}, {-1,-1} };
        const int direccionesNegras[2][2] = { {1,1}, {1,-1} };

        const int (*direcciones)[2];
        int numDirecciones = 2;

        if (fitxaOrigen->getColor() == COLOR_BLANC) 
            direcciones = direccionesBlancas;
        else 
            direcciones = direccionesNegras;
        
        for (int d = 0; d < numDirecciones; d++) {
            int dirFila = direcciones[d][0];
            int dirCol = direcciones[d][1];
            int novaFila = origen.getFila() + dirFila;
            int novaCol = origen.getColumna() + dirCol;

            if (esDestiDinsLimits(novaFila, novaCol)) {
                if (m_tauler[novaFila][novaCol] == nullptr) {
                    posicionsPossibles.push_back(Posicio(novaFila, novaCol));
                }
                // Añadimos movimiento de captura como opcional
                else if (m_tauler[novaFila][novaCol]->getColor() != fitxaOrigen->getColor()) {
                    int filaCaptura = novaFila + dirFila;
                    int colCaptura = novaCol + dirCol;
                    if (esDestiDinsLimits(filaCaptura, colCaptura) &&
                        m_tauler[filaCaptura][colCaptura] == nullptr) {
                        posicionsPossibles.push_back(Posicio(filaCaptura, colCaptura));
                    }
                }
            }
        }
    }
    else if (fitxaOrigen->getTipus() == TIPUS_DAMA) {
        // Similar para damas, permitiendo todos los movimientos
        const int direccionesDama[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

        for (int d = 0; d < 4; d++) {
            int dirFila = direccionesDama[d][0];
            int dirCol = direccionesDama[d][1];
            int f = origen.getFila() + dirFila;
            int c = origen.getColumna() + dirCol;

            while (esDestiDinsLimits(f, c)) {
                if (m_tauler[f][c] == nullptr) {
                    posicionsPossibles.push_back(Posicio(f, c));
                    f += dirFila;
                    c += dirCol;
                }
                else if (m_tauler[f][c]->getColor() != fitxaOrigen->getColor()) {
                    int filaCaptura = f + dirFila;
                    int colCaptura = c + dirCol;
                    if (esDestiDinsLimits(filaCaptura, colCaptura) &&
                        m_tauler[filaCaptura][colCaptura] == nullptr) {
                        posicionsPossibles.push_back(Posicio(filaCaptura, colCaptura));
                    }
                    break;
                }
                else {
                    break;
                }
            }
        }
    }
}

void Tauler::eliminaFitxa(const Posicio& pos) {
    if (pos.esValida() && m_tauler[pos.getFila()][pos.getColumna()] != nullptr) {
        delete m_tauler[pos.getFila()][pos.getColumna()];
        m_tauler[pos.getFila()][pos.getColumna()] = nullptr;
    }
}

bool Tauler::movimentDamaValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, const Fitxa& fitxaOrigen, bool& esCaptura)
{
    int deltaFila = filaDesti - filaOrigen;
    int deltaCol = colDesti - colOrigen;
    int dirFila = calculaDireccio(deltaFila);
    int dirCol = calculaDireccio(deltaCol);

    int f = filaOrigen + dirFila;
    int c = colOrigen + dirCol;
    bool fichaEncontrada = false;

    // Verifica todo el camino de la dama
    while (f != filaDesti && c != colDesti) {
        if (m_tauler[f][c] != nullptr) {
            if (fichaEncontrada || m_tauler[f][c]->getColor() == fitxaOrigen.getColor())
                return false;
            fichaEncontrada = true;
            esCaptura = true;
        }
        f += dirFila;
        c += dirCol;
    }

    return true;
}

// Actualiza todos los movimientos validos posibles
void Tauler::actualitzaMovimentsValids() {
    for (int fila = 0; fila < N_FILES; fila++) {
        for (int col = 0; col < N_COLUMNES; col++) {
            if (m_tauler[fila][col] != nullptr) {
                m_tauler[fila][col]->netejaMovimentsValids();
                vector<Moviment> movimentsValids;
                vector<Moviment> movimentsPendents;

                Moviment movimentInicial;
                movimentInicial.afegirPosicio(Posicio(fila, col));
                movimentsPendents.push_back(movimentInicial);

                bool hiHaCaptures = false;
                bool posicionsVisitades[N_FILES][N_COLUMNES] = { false };

                // Procesa movimientos pendientes
                while (!movimentsPendents.empty()) {
                    Moviment movimentActual = movimentsPendents.back();
                    movimentsPendents.pop_back();
                    Posicio posicioActual = movimentActual.getUltimaPosicio();

                    if (!posicionsVisitades[posicioActual.getFila()][posicioActual.getColumna()]) {

                        posicionsVisitades[posicioActual.getFila()][posicioActual.getColumna()] = true;

                        vector<Posicio> posicionsPossibles;
                        getPosicionsPossibles(posicioActual, posicionsPossibles);
                        bool haAfegitMoviment = false;

                        // Verifica cada movimiento posible
                        for (size_t i = 0; i < posicionsPossibles.size(); i++) {
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

                                    movimentsPendents.push_back(nouMoviment);
                                    haAfegitMoviment = true;
                                }
                                else if (!hiHaCaptures) {
                                    Moviment nouMoviment = movimentActual;
                                    nouMoviment.afegirPosicio(posDesti);
                                    movimentsValids.push_back(nouMoviment);
                                }
                            }
                        }

                        // Si no hay mas capturas posibles, añade el movimiento
                        if (!haAfegitMoviment && movimentActual.getNumPosicions() > 1 && hiHaCaptures)
                            movimentsValids.push_back(movimentActual);
                        
                    }
                }

                // Anade todos los movimientos validos encontrados
                for (size_t i = 0; i < movimentsValids.size(); i++) 
                    m_tauler[fila][col]->afegeixMovimentValid(movimentsValids[i]);
                
            }
        }
    }
}


// Mueve una ficha en el tablero
bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti, bool& esCaptura) {
    esCaptura = false;
    if (!origen.esValida() || !desti.esValida()) return false;
    if (m_tauler[origen.getFila()][origen.getColumna()] == nullptr) return false;

    Fitxa* fitxa = m_tauler[origen.getFila()][origen.getColumna()];

    // Verificar dirección válida para fichas normales
    if (fitxa->getTipus() == TIPUS_NORMAL) {
        int deltaFila = desti.getFila() - origen.getFila();

        // Las fichas blancas solo pueden moverse hacia arriba (deltaFila negativo)
        if (fitxa->getColor() == COLOR_BLANC && deltaFila >= 0) {
            return false;
        }
        // Las fichas negras solo pueden moverse hacia abajo (deltaFila positivo)
        if (fitxa->getColor() == COLOR_NEGRE && deltaFila <= 0) {
            return false;
        }
    }

    if (!esMovimentValid(origen.getFila(), origen.getColumna(), desti.getFila(), desti.getColumna(), esCaptura)) {
        return false;
    }

    // Procesa captura de fichas
    if (esCaptura) {
        int deltaFila = desti.getFila() - origen.getFila();
        int deltaCol = desti.getColumna() - origen.getColumna();
        int dirFila = calculaDireccio(deltaFila);
        int dirCol = calculaDireccio(deltaCol);

        // Elimina fichas contrarias en el camino
        int fila = origen.getFila() + dirFila;
        int col = origen.getColumna() + dirCol;
        while (fila != desti.getFila() && col != desti.getColumna()) {
            if (m_tauler[fila][col] != nullptr) {
                delete m_tauler[fila][col];
                m_tauler[fila][col] = nullptr;
            }
            fila += dirFila;
            col += dirCol;
        }
    }

    // Mueve la ficha
    m_tauler[desti.getFila()][desti.getColumna()] = fitxa;
    m_tauler[origen.getFila()][origen.getColumna()] = nullptr;

    // Promoción a dama
    if (fitxa->getTipus() == TIPUS_NORMAL) {
        bool esBlanca = (fitxa->getColor() == COLOR_BLANC);
        bool filaFinal = false;

        if (esBlanca && desti.getFila() == 0) 
            filaFinal = true;
        
        if (!esBlanca && desti.getFila() == N_FILES - 1) 
            filaFinal = true;

        if (filaFinal) 
            fitxa->setTipus(TIPUS_DAMA);
    }

    return true;
}
// Convierte el tablero a string para mostrar
string Tauler::toString() const
{
    string resultat;
    string files = "87654321";  // Numeracion de filas

    for (int i = 0; i < N_FILES; i++)
    {
        resultat += files[i];
        resultat += ": ";
        for (int j = 0; j < N_COLUMNES; j++)
        {
            if (m_tauler[i][j] == nullptr) {
                resultat += "_ ";  // Casilla vacia
            }
            else {
                if (m_tauler[i][j]->getColor() == COLOR_NEGRE) {
                    if (m_tauler[i][j]->getTipus() == TIPUS_NORMAL) {
                        resultat += "X ";  // Ficha negra normal
                    }
                    else {
                        resultat += "R ";  // Dama negra
                    }
                }
                else {
                    if (m_tauler[i][j]->getColor() == COLOR_BLANC) {
                        if (m_tauler[i][j]->getTipus() == TIPUS_NORMAL) {
                            resultat += "O ";  // Ficha blanca normal
                        }
                        else {
                            resultat += "D ";  // Dama blanca
                        }
                    }
                }
            }
        }
        resultat += "\n";
    }
    resultat += "   a b c d e f g h";  // Letras de columnas
    return resultat;
}


// Busca capturas recursivamente
bool Tauler::buscarCapturesRecursivas(const Posicio& origen, vector<Posicio>& posicionsPossibles)
{
    if (m_tauler[origen.getFila()][origen.getColumna()] == nullptr) return false;

    Fitxa* fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];

    bool hiHaCaptures = false;
    bool posicionesProcesadas[N_FILES][N_COLUMNES] = { false };

    buscarCapturesAux(origen, posicionesProcesadas, posicionsPossibles, hiHaCaptures);

    return hiHaCaptures;
}

// Funcion auxiliar recursiva para buscar capturas
void Tauler::buscarCapturesAux(const Posicio& pos, bool posicionesProcesadas[N_FILES][N_COLUMNES],
    vector<Posicio>& posicionsPossibles, bool& hiHaCaptures)
{
    if (m_tauler[pos.getFila()][pos.getColumna()] == nullptr) return;

    Fitxa* fitxaOrigen = m_tauler[pos.getFila()][pos.getColumna()];
    const int direcciones[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

    posicionesProcesadas[pos.getFila()][pos.getColumna()] = true;

    if (fitxaOrigen->getTipus() == TIPUS_NORMAL) {
        for (int d = 0; d < 4; d++) {
            int dirFila = direcciones[d][0];
            int dirCol = direcciones[d][1];
            int filaIntermedia = pos.getFila() + dirFila;
            int colIntermedia = pos.getColumna() + dirCol;
            int filaDesti = pos.getFila() + 2 * dirFila;
            int colDesti = pos.getColumna() + 2 * dirCol;

            bool posicionValida = (filaIntermedia >= 0 && filaIntermedia < N_FILES &&
                colIntermedia >= 0 && colIntermedia < N_COLUMNES &&
                filaDesti >= 0 && filaDesti < N_FILES &&
                colDesti >= 0 && colDesti < N_COLUMNES);

            if (posicionValida) {
                bool piezaContraria = (m_tauler[filaIntermedia][colIntermedia] != nullptr) &&
                    (m_tauler[filaIntermedia][colIntermedia]->getColor() != fitxaOrigen->getColor());
                bool destinoLibre = (m_tauler[filaDesti][colDesti] == nullptr);

                if (piezaContraria && destinoLibre) {
                    hiHaCaptures = true;

                    if (!posicionesProcesadas[filaDesti][colDesti]) {
                        posicionesProcesadas[filaDesti][colDesti] = true;
                        posicionsPossibles.push_back(Posicio(filaDesti, colDesti));
                    }

                    Fitxa* piezaCapturada = m_tauler[filaIntermedia][colIntermedia];
                    Fitxa* piezaOriginal = m_tauler[pos.getFila()][pos.getColumna()];

                    m_tauler[filaIntermedia][colIntermedia] = nullptr;
                    m_tauler[pos.getFila()][pos.getColumna()] = nullptr;
                    m_tauler[filaDesti][colDesti] = piezaOriginal;

                    buscarCapturesAux(Posicio(filaDesti, colDesti), posicionesProcesadas, posicionsPossibles, hiHaCaptures);

                    m_tauler[filaIntermedia][colIntermedia] = piezaCapturada;
                    m_tauler[pos.getFila()][pos.getColumna()] = piezaOriginal;
                    m_tauler[filaDesti][colDesti] = nullptr;
                }
            }
        }
    }
    else if (fitxaOrigen->getTipus() == TIPUS_DAMA) {
        for (int d = 0; d < 4; d++) {
            int dirFila = direcciones[d][0];
            int dirCol = direcciones[d][1];
            int f = pos.getFila() + dirFila;
            int c = pos.getColumna() + dirCol;
            bool fichaContrariaEncontrada = false;
            int filaCaptura = -1, colCaptura = -1;
            bool busquedaActiva = true;

            while (esDestiDinsLimits(f, c) && busquedaActiva) {
                if (m_tauler[f][c] != nullptr) {
                    if (m_tauler[f][c]->getColor() == fitxaOrigen->getColor()) {
                        busquedaActiva = false;
                    }
                    else if (!fichaContrariaEncontrada) {
                        fichaContrariaEncontrada = true;
                        filaCaptura = f;
                        colCaptura = c;
                    }
                    else {
                        busquedaActiva = false;
                    }
                }
                else {
                    if (fichaContrariaEncontrada) {
                        hiHaCaptures = true;

                        if (!posicionesProcesadas[f][c]) {
                            posicionesProcesadas[f][c] = true;
                            posicionsPossibles.push_back(Posicio(f, c));
                        }

                        Fitxa* piezaCapturada = m_tauler[filaCaptura][colCaptura];
                        Fitxa* piezaOriginal = m_tauler[pos.getFila()][pos.getColumna()];

                        m_tauler[filaCaptura][colCaptura] = nullptr;
                        m_tauler[pos.getFila()][pos.getColumna()] = nullptr;
                        m_tauler[f][c] = piezaOriginal;

                        buscarCapturesAux(Posicio(f, c), posicionesProcesadas, posicionsPossibles, hiHaCaptures);

                        m_tauler[filaCaptura][colCaptura] = piezaCapturada;
                        m_tauler[pos.getFila()][pos.getColumna()] = piezaOriginal;
                        m_tauler[f][c] = nullptr;
                    }
                }
                f += dirFila;
                c += dirCol;
            }
        }
    }
}

void Tauler::bufar(ColorFitxa colorJugador) {
    // Vector para almacenar fichas que deben ser eliminadas
    vector<Posicio> fichasAEliminar;

    // Recorrer todo el tablero buscando fichas del jugador actual
    // que tengan capturas disponibles pero no capturaron
    for (int fila = 0; fila < N_FILES; fila++) {
        for (int col = 0; col < N_COLUMNES; col++) {
            if (m_tauler[fila][col] != nullptr &&
                m_tauler[fila][col]->getColor() == colorJugador) {

                // Verificar si esta ficha tiene capturas disponibles
                bool tieneCapturas = false;
                vector<Posicio> movimentsPossibles;
                getPosicionsPossibles(Posicio(fila, col), movimentsPossibles);

                for (size_t i = 0; i < movimentsPossibles.size(); i++) {
                    bool esCaptura = false;
                    if (esMovimentValid(fila, col, movimentsPossibles[i].getFila(), movimentsPossibles[i].getColumna(), esCaptura) && esCaptura) {
                        tieneCapturas = true;
                        break;
                    }
                }

                // Si tiene capturas disponibles, marcarla para eliminar
                if (tieneCapturas) {
                    fichasAEliminar.push_back(Posicio(fila, col));
                }
            }
        }
    }

    // Si hay exactamente dos fichas para eliminar, eliminar solo la que ha movido o una de ellas
    if (fichasAEliminar.size() == 2) {
        // Eliminar la primera ficha de la lista
        eliminaFitxa(fichasAEliminar[0]);
    }
    else {
        // Eliminar todas las fichas marcadas en otros casos
        for (size_t i = 0; i < fichasAEliminar.size(); i++) {
            eliminaFitxa(fichasAEliminar[i]);
        }
    }
}
