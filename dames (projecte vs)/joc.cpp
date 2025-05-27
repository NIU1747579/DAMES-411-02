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
    m_jugadorActual = COLOR_BLANC;
    m_tauler.inicialitza(nomFitxerTauler);
    m_fitxaSeleccionada = Posicio(); 
    m_finalPartida = false;

    if (mode == MODE_JOC_REPLAY) {
        ifstream fitxerMoviments(nomFitxerMoviments);
        if(fitxerMoviments.is_open()) 
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

bool Joc::actualitzaModeNormal(int mousePosX, int mousePosY, bool mouseStatus)
{
    // Convertim coordenades a posicio del tauler
    int col = (mousePosX - POS_X_TAULER - CASELLA_INICIAL_X) / AMPLADA_CASELLA;
    int fila = (mousePosY - POS_Y_TAULER - CASELLA_INICIAL_Y) / ALCADA_CASELLA;

    Posicio posClic = Posicio(); 
    static Fitxa* fitxa = nullptr; 

    // Comprovem que el clic es dins del tauler
    if (fila < 0 || fila >= NUM_FILES_TAULER || col < 0 || col >= NUM_COLS_TAULER
        && mouseStatus) 
    {
  
        
        m_fitxaSeleccionada = Posicio(fila, col);
        m_movimentsValids.clear();

        fitxa = m_tauler.getFitxa(fila, col);
        posClic = Posicio(fila, col);

        m_fitxaSeleccionada = posClic;
        m_movimentsValids.clear();
        m_tauler.getPosicionsPossibles(m_fitxaSeleccionada, m_movimentsValids);


        return false;
    }

    // Deseleccionem si es fa clic a la mateixa fitxa
    if (posClic == m_fitxaSeleccionada) {
        m_fitxaSeleccionada = Posicio();
        m_movimentsValids.clear();


        return false;
    }

    // Comprovem moviments valids
    for (size_t i = 0; i < m_movimentsValids.size(); i++) {
        if (m_movimentsValids[i] == posClic) {
            // Movem la fitxa
            if (m_tauler.mouFitxa(m_fitxaSeleccionada, posClic)) {
                // Guardem el moviment a la cua
                m_cua.afegeixMoviment(m_fitxaSeleccionada.toString(), posClic.toString());

                // COmprovem si fi de partida
                if (m_tauler.comprovarFiPartida(m_guanyador)) {
                    m_finalPartida = true;
                }
                canviarTorn();

            }
        }
    }
    // Si cliquem una altra fitxa del mateix jugador, canviem la seleccio per la nova fitxa
    if (fitxa != nullptr && fitxa->getColor() == m_jugadorActual) {
        m_fitxaSeleccionada = posClic;
        m_movimentsValids.clear();
        m_tauler.getPosicionsPossibles(m_fitxaSeleccionada, m_movimentsValids);

    }
    else {
        m_fitxaSeleccionada = Posicio();
        m_movimentsValids.clear();

    }
    return false;
}

bool Joc::actualitzaModeReplay(int mousePosX, int mousePosY, bool mouseStatus) {

    int col = (mousePosX - POS_X_TAULER - CASELLA_INICIAL_X) / AMPLADA_CASELLA;
    int fila = (mousePosY - POS_Y_TAULER - CASELLA_INICIAL_Y) / ALCADA_CASELLA;

    if (fila < 0 || fila >= NUM_FILES_TAULER || col < 0 || col >= NUM_COLS_TAULER) return false;
    
    if (m_finalPartida) return false;
    

    // Si hi ha moviments a la cua
    if (!m_cua.esBuida()) {
        // Agafem el seguent moviment de la cu moviments
        Moviment moviment = m_cua.treuMoviment();
        Posicio origen = moviment.getPosicio(0);
        Posicio desti = moviment.getPosicio(1);

        // Fem el moviment de la fitxa
        if (m_tauler.mouFitxa(origen, desti)) {
            // Comprovem si fi de partida
            if (m_tauler.comprovarFiPartida(m_guanyador))m_finalPartida = true;
            else canviarTorn();
           
        }

        return true;
    }
    else {
        // Si no hi ha mes moviment i la partida a acabat
        if (!m_finalPartida) {
            string missatge = "No es poden fer mes moviments!";
            GraphicManager::getInstance()->drawFont(FONT_RED_30, 400, 400, 1.0, missatge); 
        }
        return false;
    }
}

bool Joc::actualitza(int mousePosX, int mousePosY, bool mouseStatus) 
{
    // Mostrem fons, tauler i estat del tauler
    GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0); 
    GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);  
   
    m_tauler.visualitzaTauler(m_fitxaSeleccionada, m_movimentsValids);

    dibuixaInformacioJoc();

    bool resultat = false;
    // Nomes avanca si es fa clic dins del tauler
    if (mouseStatus)
    {
        // Gestio segons el mode de joc
        if (m_mode == MODE_JOC_NORMAL) {
            resultat = actualitzaModeNormal(mousePosX, mousePosY,mouseStatus);

            GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0); 
            GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER); 
            m_tauler.visualitzaTauler(m_fitxaSeleccionada, m_movimentsValids); 
            dibuixaInformacioJoc(); 

            return resultat; 

        }
        else if (m_mode == MODE_JOC_REPLAY) {
            resultat = actualitzaModeReplay(mousePosX, mousePosY, mouseStatus);
            GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
            GraphicManager::getInstance()->drawSprite(GRAFIC_TAULER, POS_X_TAULER, POS_Y_TAULER);
            m_tauler.visualitzaTauler(m_fitxaSeleccionada, m_movimentsValids);
            dibuixaInformacioJoc();

            return resultat;
        }
    }

	return false;
}

void Joc::finalitza() {
   // Guardar la cua de moviments al fitxer 'nomFitxerMoviments'
   if (m_mode == MODE_JOC_NORMAL)
       m_cua.guardaMoviments("nomFitxerMoviments");
    // Netegem la cua de moviments
    m_cua.neteja();

}



