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
                delete[] m_tauler;
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
    if (fila >= 0 && fila < NUM_FILES_TAULER && col >= 0 && col < NUM_COLS_TAULER) 
        return m_tauler[fila][col]; 
    else
        return nullptr;
}

void Tauler::visualitzaTauler(const Posicio& fitxaSeleccionada, const vector<Posicio>& movimentsValids) {
    for (int fila = 0; fila < NUM_FILES_TAULER; ++fila) { 
        for (int col = 0; col < NUM_COLS_TAULER; ++col) { 
            if (m_tauler[fila][col] != nullptr) {
                int x = POS_X_TAULER + CASELLA_INICIAL_X + (col * AMPLADA_CASELLA);
                int y = POS_Y_TAULER + CASELLA_INICIAL_Y + (fila * ALCADA_CASELLA); 

                m_tauler[fila][col]->visualitzaFitxa(x, y); 

                // Marcar la fitxa seleccionada amb un cercle groc
                if (fitxaSeleccionada.esValida() &&
                    fitxaSeleccionada.getFila() == fila &&
                    fitxaSeleccionada.getColumna() == col) {
                    GraphicManager::getInstance()->drawSprite(GRAFIC_POSICIO_VALIDA, x, y);
                }

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
                if (m_tauler[i][j]->getColor() == COLOR_BLANC) {
                    blancTeFitxes = true;
                }
                else {
                    negreTeFitxes = true;
                }
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
bool Tauler::movimentFitxaNormalValid(int filaOrigen, int colOrigen, int filaDesti, int colDesti, const Fitxa& fitxaOrigen, bool& esCaptura)
{
    // Calcula diferencia de posiciones entre destino y origen
    int deltaFila = filaDesti - filaOrigen;
    int deltaCol = colDesti - colOrigen;

    bool direccioCorrecta;

    // Determina direccion valida segun color
    if (fitxaOrigen.getColor() == COLOR_BLANC) {
        direccioCorrecta = (deltaFila < 0);  // Blancas mueven hacia arriba
    }
    else {
        direccioCorrecta = (deltaFila > 0);  // Negras mueven hacia abajo
    }

    // Movimiento simple (1 casilla)
    if (abs(deltaFila) == 1 && abs(deltaCol) == 1) return direccioCorrecta;

    // Movimiento de captura (2+ casillas)
    if (abs(deltaFila) >= 2 && abs(deltaCol) >= 2 && abs(deltaFila) == abs(deltaCol)) {
        int dirFila = calculaDireccio(deltaFila);
        int dirCol = calculaDireccio(deltaCol);

        int fitxesContraries = 0;
        int fila = filaOrigen + dirFila;
        int col = colOrigen + dirCol;

        // Verifica fichas contrarias en el camino
        while (fila != filaDesti && col != colDesti) {
            if (m_tauler[fila][col] != nullptr) { 
                fitxesContraries++;
                if (m_tauler[fila][col]->getColor() == fitxaOrigen.getColor()) return false; 
            }
            fila += dirFila;
            col += dirCol;
        }

        // Solo permite capturar una ficha por movimiento
        if (fitxesContraries == abs(deltaFila) / 2) {
            esCaptura = true;
            return true;
        }
    }
    return false;
}

// Funcion para validar movimiento de dama
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
                        if (!haAfegitMoviment && movimentActual.getNumPosicions() > 1 && hiHaCaptures) {
                            movimentsValids.push_back(movimentActual); 
                        }
                    }
                }

                // Anade todos los movimientos validos encontrados
                for (size_t i = 0; i < movimentsValids.size(); i++) { 
                    m_tauler[fila][col]->afegeixMovimentValid(movimentsValids[i]);
                }
            }
        }
    }
}

// Mueve una ficha en el tablero
bool Tauler::mouFitxa(const Posicio& origen, const Posicio& desti) 
{
    if (!origen.esValida() || !desti.esValida()) return false; 
    if (m_tauler[origen.getFila()][origen.getColumna()] == nullptr) return false; 

    bool esCaptura = false;
    if (!esMovimentValid(origen.getFila(), origen.getColumna(), desti.getFila(), desti.getColumna(), esCaptura)) {
        return false;
    }

    // Procesa captura de fichas
    if (esCaptura) {
        int deltaFila = desti.getFila() - origen.getFila();
        int deltaCol = desti.getColumna() - origen.getColumna();

        int dirFila = calculaDireccio(deltaFila);
        int dirCol = calculaDireccio(deltaCol);

        int fila = origen.getFila() + dirFila;
        int col = origen.getColumna() + dirCol;

        Fitxa* fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()]; 
        ColorFitxa colorOrigen = fitxaOrigen->getColor(); 

        // Elimina fichas contrarias en el camino
        while (fila != desti.getFila() && col != desti.getColumna()) {
            if (m_tauler[fila][col] != nullptr && 
                m_tauler[fila][col]->getColor() != colorOrigen) {
                delete m_tauler[fila][col]; 
                m_tauler[fila][col] = nullptr; 
            }
            fila += dirFila;
            col += dirCol;
        }
    }

    // Mueve la ficha
    m_tauler[desti.getFila()][desti.getColumna()] = m_tauler[origen.getFila()][origen.getColumna()]; 
    m_tauler[origen.getFila()][origen.getColumna()] = nullptr; 

    // Promocion a dama
    if (m_tauler[desti.getFila()][desti.getColumna()] != nullptr) 
    
    {
        bool esBlanca = (m_tauler[desti.getFila()][desti.getColumna()]->getColor() == COLOR_BLANC);

        bool arribaFinal;

        if (esBlanca) arribaFinal = (desti.getFila() == 0);  // Blancas coronan en fila 0
        else arribaFinal = (desti.getFila() == N_FILES - 1);  // Negras coronan en ultima fila

        if (arribaFinal) m_tauler[desti.getFila()][desti.getColumna()]->setTipus(TIPUS_DAMA);

        // Verifica si desde la nueva posicion se pueden capturar mas fichas
        bool potCapturarDesdeNovaPosicio = false;
        vector<Posicio> posicionsPossibles;
        getPosicionsPossibles(desti, posicionsPossibles);

        for (size_t i = 0; i < posicionsPossibles.size(); i++) {
            Posicio& pos = posicionsPossibles[i];
            bool esCapturaPossible = false;

            if (esMovimentValid(desti.getFila(), desti.getColumna(), pos.getFila(),
                pos.getColumna(), esCapturaPossible)) {

                if (esCapturaPossible) potCapturarDesdeNovaPosicio = true;
            }
        }

        // Regla de captura obligatoria
        if (!esCaptura || (esCaptura && potCapturarDesdeNovaPosicio)) {
            bool hihaAltresCaptures = false;

            // Busca en todo el tablero
            for (int i = 0; i < N_FILES; i++) {
                for (int j = 0; j < N_COLUMNES; j++) {
                    if (m_tauler[i][j] != nullptr &&
                        m_tauler[i][j]->getColor() == m_tauler[desti.getFila()][desti.getColumna()]->getColor()) {

                        vector<Posicio> posicionesPosibles;
                        getPosicionsPossibles(Posicio(i, j), posicionesPosibles);

                        for (size_t x = 0; x < posicionesPosibles.size(); ++x) {
                            const Posicio& pos = posicionesPosibles[x];
                            bool esUnaCaptura = false;
                            if (esMovimentValid(i, j, pos.getFila(), pos.getColumna(), esUnaCaptura)) {
                                if (esUnaCaptura) {
                                    hihaAltresCaptures = true;
                                    delete m_tauler[i][j];
                                    m_tauler[i][j] = nullptr;
                                    actualitzaMovimentsValids();
                                    return true;
                                }
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

// Obtiene posiciones posibles para una ficha
void Tauler::getPosicionsPossibles(const Posicio& origen, vector<Posicio>& posicionsPossibles)
{
    posicionsPossibles.clear();
    if (!origen.esValida() || m_tauler[origen.getFila()][origen.getColumna()] == nullptr) {
        return;
    }

    Fitxa* fitxaOrigen = m_tauler[origen.getFila()][origen.getColumna()];
    const int direcciones[4][2] = { {1,1}, {1,-1}, {-1,1}, {-1,-1} };

    // Busca capturas posibles
    vector<Posicio> captures;
    bool hiHaCaptures = buscarCapturesRecursivas(origen, captures);

    for (size_t i = 0; i < captures.size(); i++) { 
        posicionsPossibles.push_back(captures[i]); 
    }

    // Anade movimientos simples si no hay capturas
    if (fitxaOrigen->getTipus() == TIPUS_NORMAL) {
        int direccionFila;
        if (fitxaOrigen->getColor() == COLOR_BLANC) {
            direccionFila = -1;  // Blancas mueven hacia arriba
        }
        else direccionFila = 1;   // Negras mueven hacia abajo
        

        // Diagonal izquierda
        int novaFila = origen.getFila() + direccionFila;
        int novaCol = origen.getColumna() - 1;
        if (esDestiDinsLimits(novaFila, novaCol) &&
            m_tauler[novaFila][novaCol] == nullptr) {
            posicionsPossibles.push_back(Posicio(novaFila, novaCol));
        }

        // Diagonal derecha
        novaCol = origen.getColumna() + 1;
        if (esDestiDinsLimits(novaFila, novaCol) && 
            m_tauler[novaFila][novaCol] == nullptr) {
            posicionsPossibles.push_back(Posicio(novaFila, novaCol)); 
        }
    }
    else if (fitxaOrigen->getTipus() == TIPUS_DAMA) {
        // Movimientos de dama en todas direcciones
        for (size_t d = 0; d < 4; d++) { 
            int dirFila = direcciones[d][0];
            int dirCol = direcciones[d][1];
            int f = origen.getFila() + dirFila;
            int c = origen.getColumna() + dirCol;

            while (esDestiDinsLimits(f, c) && m_tauler[f][c] == nullptr) { 
                posicionsPossibles.push_back(Posicio(f, c)); 
                f += dirFila;
                c += dirCol;
            }
        }
    }
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
        // Logica de captura para fichas normales
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

                    // Simula y restaura captura para buscar capturas multiples
                    Fitxa* piezaCapturada = m_tauler[filaIntermedia][colIntermedia]; 
                    Fitxa* piezaOriginal = m_tauler[pos.getFila()][pos.getColumna()]; 

                    m_tauler[filaIntermedia][colIntermedia] = nullptr;
                    m_tauler[pos.getFila()][pos.getColumna()] = nullptr;
                    m_tauler[filaDesti][colDesti] = piezaOriginal;

                    buscarCapturesAux(Posicio(filaDesti, colDesti), posicionesProcesadas, 
                        posicionsPossibles, hiHaCaptures); 

                    m_tauler[filaIntermedia][colIntermedia] = piezaCapturada; 
                    m_tauler[pos.getFila()][pos.getColumna()] = piezaOriginal; 
                    delete m_tauler[filaDesti][colDesti]; 
                    m_tauler[filaDesti][colDesti] = nullptr; 
                } 
            }
        }
    }
    else if (fitxaOrigen->getTipus() == TIPUS_DAMA) {
        // Logica de captura para damas
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
                    else 
                        busquedaActiva = false;
                    
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

                        buscarCapturesAux(Posicio(f, c), posicionesProcesadas, 
                            posicionsPossibles, hiHaCaptures); 

                        m_tauler[filaCaptura][colCaptura] = piezaCapturada; 
                        m_tauler[pos.getFila()][pos.getColumna()] = piezaOriginal; 
                        delete m_tauler[f][c]; 
                        m_tauler[f][c] = nullptr; 

                    }
                }
                f += dirFila;
                c += dirCol;
            }
        }
    }
}
