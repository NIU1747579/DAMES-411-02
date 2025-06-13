

//
//  CurrentGame.cpp
//  LearnChess
//
//  Created by Enric Vergara on 21/2/22.
//

#include "joc.hpp"
#include <iostream>
#include <fstream>
#include "GraphicManager.h"


void Joc::inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments) {
    m_mode = mode;
    m_tauler.inicialitza(nomFitxerTauler); 
    m_nomFitxerMoviments = nomFitxerMoviments; 

    m_jugadorActual = COLOR_BLANC;
    m_fitxaSeleccionada = Posicio();
    m_ultimaPosicioDesti = Posicio();

    m_finalPartida = false;
    m_ultimMovimentEsCaptura = false; // Inicializar

    if (mode == MODE_JOC_REPLAY) {
         m_esperantPrimerClic = true;
        ifstream fitxerMoviments(nomFitxerMoviments);
        if (fitxerMoviments.is_open())
        {
            string origen, desti;
            while (fitxerMoviments >> origen >> desti)
            {
                m_cua.afegeixMoviment(origen, desti);
            }
        }
        fitxerMoviments.close();
    }
}

void Joc::canviarTorn() {
    if (m_jugadorActual == COLOR_BLANC) m_jugadorActual = COLOR_NEGRE;
    else m_jugadorActual = COLOR_BLANC;
}

bool Joc::actualitzaModeNormal(int mousePosX, int mousePosY, bool mouseStatus) {
    if (!mouseStatus) return false;

    // Convertir coordenadas a posición del tablero
    int col = (mousePosX - POS_X_TAULER - CASELLA_INICIAL_X) / AMPLADA_CASELLA;
    int fila = (mousePosY - POS_Y_TAULER - CASELLA_INICIAL_Y) / ALCADA_CASELLA;
    Posicio posClic(fila, col);

    // Verificar que el clic esté dentro del tablero
    if (fila < 0 || fila >= NUM_FILES_TAULER || col < 0 || col >= NUM_COLS_TAULER) {
        return false;
    }

    Fitxa* fitxa = m_tauler.getFitxa(fila, col);

    // NUEVA VARIABLE: Verificar si estamos en medio de capturas múltiples
    bool enCapturaMultiple = m_fitxaSeleccionada.esValida() && !m_movimentsValids.empty();

    // Verificar si los movimientos válidos actuales son todos capturas
    bool movimentsValidsSonCaptures = false;
    if (enCapturaMultiple) {
        for (size_t i = 0; i < m_movimentsValids.size(); i++) {
            bool esCaptura = false;
            m_tauler.esMovimentValid(m_fitxaSeleccionada.getFila(), m_fitxaSeleccionada.getColumna(),
                m_movimentsValids[i].getFila(), m_movimentsValids[i].getColumna(), esCaptura);
            if (esCaptura) {
                movimentsValidsSonCaptures = true;
                break;
            }
        }
    }

    // Si no hay ficha seleccionada y se hace clic en una ficha del jugador actual
    if (!m_fitxaSeleccionada.esValida()) {
        if (fitxa != nullptr && fitxa->getColor() == m_jugadorActual) {
            // Verificar si hay capturas obligatorias
            bool hiHaCaptures = false;
            vector<Posicio> posicionsPossibles;
            m_tauler.getPosicionsPossibles(posClic, posicionsPossibles);

            // Filtrar solo movimientos que sean capturas
            for (size_t i = 0; i < posicionsPossibles.size(); i++) {
                bool esCaptura = false;
                if (m_tauler.esMovimentValid(posClic.getFila(), posClic.getColumna(),
                    posicionsPossibles[i].getFila(), posicionsPossibles[i].getColumna(), esCaptura) && esCaptura) {
                    hiHaCaptures = true;
                    break;
                }
            }

            // Si hay capturas disponibles, solo permitir seleccionar fichas con capturas
            if (hiHaCaptures) {
                bool teCaptures = false;
                for (size_t i = 0; i < posicionsPossibles.size(); i++) {
                    bool esCaptura = false;
                    if (m_tauler.esMovimentValid(posClic.getFila(), posClic.getColumna(),
                        posicionsPossibles[i].getFila(), posicionsPossibles[i].getColumna(), esCaptura) && esCaptura) {
                        teCaptures = true;
                        break;
                    }
                }
                if (!teCaptures) {
                    return false; // No permitir seleccionar esta ficha si no tiene capturas
                }
            }

            m_fitxaSeleccionada = posClic;
            m_tauler.getPosicionsPossibles(m_fitxaSeleccionada, m_movimentsValids);
        }
        return false;
    }

    // Si se hace clic en la ficha seleccionada, deseleccionar SOLO si no estamos en captura múltiple
    if (posClic == m_fitxaSeleccionada) {
        if (!movimentsValidsSonCaptures) {
            m_fitxaSeleccionada = Posicio();
            m_movimentsValids.clear();
        }
        return false;
    }

    // Verificar si el movimiento es válido
    for (size_t i = 0; i < m_movimentsValids.size(); i++) {
        if (m_movimentsValids[i] == posClic) {
            // Verificar si había capturas obligatorias ANTES del movimiento
            bool hiHaCapturesObligatories = false;
            for (int f = 0; f < NUM_FILES_TAULER && !hiHaCapturesObligatories; f++) {
                for (int c = 0; c < NUM_COLS_TAULER && !hiHaCapturesObligatories; c++) {
                    if (m_tauler.getFitxa(f, c) != nullptr &&
                        m_tauler.getFitxa(f, c)->getColor() == m_jugadorActual) {

                        vector<Posicio> posicionsPossibles;
                        m_tauler.getPosicionsPossibles(Posicio(f, c), posicionsPossibles);

                        for (size_t j = 0; j < posicionsPossibles.size(); j++) {
                            bool esCaptura = false;
                            if (m_tauler.esMovimentValid(f, c, posicionsPossibles[j].getFila(), posicionsPossibles[j].getColumna(), esCaptura) && esCaptura) {
                                hiHaCapturesObligatories = true;
                                break;
                            }
                        }
                    }
                }
            }

            bool esCaptura = false;
            if (m_tauler.mouFitxa(m_fitxaSeleccionada, posClic, esCaptura)) {
                m_cua.afegeixMoviment(m_fitxaSeleccionada.toString(), posClic.toString());

                // Si fue una captura, verificar si hay más capturas posibles
                if (esCaptura) {
                    m_movimentsValids.clear();
                    m_tauler.getPosicionsPossibles(posClic, m_movimentsValids);

                    // Filtrar solo movimientos que sean capturas
                    vector<Posicio> movimentsCaptura;
                    for (size_t k = 0; k < m_movimentsValids.size(); k++) {
                        bool esCapturaPotencial = false;
                        m_tauler.esMovimentValid(posClic.getFila(), posClic.getColumna(),
                            m_movimentsValids[k].getFila(), m_movimentsValids[k].getColumna(), esCapturaPotencial);
                        if (esCapturaPotencial) {
                            movimentsCaptura.push_back(m_movimentsValids[k]);
                        }
                    }

                    m_movimentsValids = movimentsCaptura;

                    // Si no hay más capturas posibles, cambiar turno
                    if (m_movimentsValids.empty()) {
                        if (m_tauler.comprovarFiPartida(m_guanyador)) {
                            m_finalPartida = true;
                        }
                        else {
                            canviarTorn();
                        }
                        m_fitxaSeleccionada = Posicio();
                    }
                    else {
                        // Mantener la ficha seleccionada para continuar capturando
                        m_fitxaSeleccionada = posClic;
                    }
                }
                else {
                    // Si no fue captura pero había capturas obligatorias, bufar
                    if (hiHaCapturesObligatories) {
                        m_tauler.bufar(m_jugadorActual);
                    }

                    if (m_tauler.comprovarFiPartida(m_guanyador)) {
                        m_finalPartida = true;
                    }
                    else {
                        canviarTorn();
                    }
                    m_fitxaSeleccionada = Posicio();
                    m_movimentsValids.clear();
                }
                return true;
            }
        }
    }

    // MODIFICACIÓN CLAVE: Si estamos en captura múltiple, NO permitir seleccionar otra ficha
    if (movimentsValidsSonCaptures) {
        return false; // Bloquear selección de otras fichas durante capturas múltiples
    }

    // Si se hace clic en otra ficha del mismo jugador, cambiar selección
    if (fitxa != nullptr && fitxa->getColor() == m_jugadorActual) {
        m_fitxaSeleccionada = posClic;
        m_movimentsValids.clear();
        m_tauler.getPosicionsPossibles(m_fitxaSeleccionada, m_movimentsValids);
    }

    return false;
}

bool Joc::actualitzaModeReplay(int mousePosX, int mousePosY, bool mouseStatus) {

    static bool anteriorClic = false;

    bool clicDinsTauler = (mousePosX >= (POS_X_TAULER + CASELLA_INICIAL_X)) &&
        (mousePosY >= POS_Y_TAULER + CASELLA_INICIAL_Y) &&
        (mousePosX <= (POS_X_TAULER + CASELLA_INICIAL_X + AMPLADA_CASELLA * NUM_COLS_TAULER)) &&
        (mousePosY <= (POS_Y_TAULER + CASELLA_INICIAL_Y + ALCADA_CASELLA * NUM_FILES_TAULER));

    if (mouseStatus && clicDinsTauler && !anteriorClic && !m_cua.esBuida())
    {
        if (m_esperantPrimerClic)
            m_esperantPrimerClic = false;
        else {
            if (!m_cua.esBuida()) {
                Moviment mov = m_cua.getPrimer();
                Posicio origen = mov.getPosicioOrigen();
                Posicio desti = mov.getPosicioDesti();
                
                // AÑADIR: Verificar si había capturas obligatorias ANTES del movimiento
                bool hiHaCapturesObligatories = false;
                for (int f = 0; f < NUM_FILES_TAULER && !hiHaCapturesObligatories; f++) {
                    for (int c = 0; c < NUM_COLS_TAULER && !hiHaCapturesObligatories; c++) {
                        if (m_tauler.getFitxa(f, c) != nullptr &&
                            m_tauler.getFitxa(f, c)->getColor() == m_jugadorActual) {

                            vector<Posicio> posicionsPossibles;
                            m_tauler.getPosicionsPossibles(Posicio(f, c), posicionsPossibles);

                            for (size_t j = 0; j < posicionsPossibles.size(); j++) {
                                bool esCaptura = false;
                                if (m_tauler.esMovimentValid(f, c, posicionsPossibles[j].getFila(), 
                                    posicionsPossibles[j].getColumna(), esCaptura) && esCaptura) {
                                    hiHaCapturesObligatories = true;
                                    break;
                                }
                            }
                        }
                    }
                }
                
                bool esCaptura = false;
                if (m_tauler.mouFitxa(origen, desti, esCaptura))
                {
                    // AÑADIR: Si no fue captura pero había capturas obligatorias, bufar
                    if (!esCaptura && hiHaCapturesObligatories) {
                        m_tauler.bufar(m_jugadorActual);
                    }
                    
                    m_cua.treuMoviment();
                    canviarTorn();
                }

                if (m_tauler.comprovarFiPartida(m_guanyador))
                    m_finalPartida = true;
            }
        }
    }
    
    if (m_cua.esBuida() && !m_esperantPrimerClic) {
        string missatge = "No hi ha mes moviments per reproduir.";
        GraphicManager::getInstance()->drawFont(FONT_GREEN_30, 50, 800, 1.0, missatge);
    }
    
    anteriorClic = mouseStatus;
    return false;
}

bool Joc::actualitza(int mousePosX, int mousePosY, bool mouseStatus)
{
    // Per controlar l'estat anterior del ratoli i aixi poder seleccionar 
    // la fitxa en un sol clic i no haver de fer mes d'un
    static bool estatAnteriorRatoli = false;
    // Mostrem fons, tauler i estat del tauler
    GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
    GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);

    m_tauler.visualitzaTauler(m_fitxaSeleccionada, m_movimentsValids);
    dibuixaInformacioJoc();

    bool resultat = false;
    // NomEs avança si es fa clic dins del tauler
    if (m_mode == MODE_JOC_NORMAL) {
        // Només processar clics dins del tauler en mode normal
        if (!estatAnteriorRatoli && (mousePosX >= (POS_X_TAULER + CASELLA_INICIAL_X)) &&
            (mousePosY >= (POS_Y_TAULER + CASELLA_INICIAL_Y)) &&
            (mousePosX <= (POS_X_TAULER + CASELLA_INICIAL_X + AMPLADA_CASELLA * NUM_COLS_TAULER)) &&
            (mousePosY <= (POS_Y_TAULER + CASELLA_INICIAL_Y + ALCADA_CASELLA * NUM_FILES_TAULER)))
        {
            resultat = actualitzaModeNormal(mousePosX, mousePosY, mouseStatus);
        }
    }
    else if (m_mode == MODE_JOC_REPLAY) {
        resultat = actualitzaModeReplay(mousePosX, mousePosY, mouseStatus);
    }

    estatAnteriorRatoli = mouseStatus;
    return resultat;
}

void Joc::finalitza() {
    // Guardar la cua de moviments al fitxer 'nomFitxerMoviments'
    if (m_mode == MODE_JOC_NORMAL)
        m_cua.guardaMoviments(m_nomFitxerMoviments);
    m_cua.neteja();
}

void Joc::dibuixaInformacioJoc() {

    string mode;
    if (m_mode == MODE_JOC_NORMAL) mode = "Mode joc: Normal";
    else if (m_mode == MODE_JOC_REPLAY) mode = "Mode joc: Replay";
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 700, 0.8, mode);

    string torn = "Torn actual: ";
    if (m_jugadorActual == COLOR_BLANC) torn += "Blanques";
    else if (m_jugadorActual == COLOR_NEGRE) torn += "Negres";
    GraphicManager::getInstance()->drawFont(FONT_WHITE_30, 50, 740, 0.8, torn);

    if (m_finalPartida) {
        string guanyador = "Guanyador: ";
        if (m_guanyador == COLOR_BLANC) guanyador += "Blanques";
        else guanyador += "Negres";

        GraphicManager::getInstance()->drawFont(FONT_RED_30, 50, 780, 1.0, guanyador);
    }
}
