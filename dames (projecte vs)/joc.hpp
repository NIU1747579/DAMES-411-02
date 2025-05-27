#ifndef JOC_H
#define JOC_H

#include <stdio.h>
#include <vector>
#include <string>
#include "info_joc.hpp"
#include "tauler.hpp"
#include "cuaMoviment.h"


using namespace std;

class Joc 
{

public:
    Joc() : m_finalPartida(false), m_mode(MODE_JOC_NONE), m_jugadorActual(COLOR_BLANC), m_guanyador(COLOR_BLANC){};
    
    bool actualitza(int mousePosX, int mousePosY, bool mouseStatus);
    void inicialitza(ModeJoc mode, const string& nomFitxerTauler, const string& nomFitxerMoviments);
    void finalitza();

    bool actualitzaModeNormal(int mousePosX, int mousePosY, bool mouseStatus);
    bool actualitzaModeReplay(int mousePosX, int mousePosY, bool mouseStatus);
    void dibuixaInformacioJoc();

    
private:
    Tauler m_tauler; // Tauler de joc
    CuaMoviments m_cua; // Cua de moviments realitzats
    ModeJoc m_mode; // Mode de joc actual
    bool m_finalPartida; // Indica si la partida ha acabat
    ColorFitxa m_guanyador; // Guanyador de la partida
    ColorFitxa m_jugadorActual; // Jugador amb el torn

    Posicio m_fitxaSeleccionada; // Posicio de la peca seleccionada
    std::vector<Posicio>m_movimentsValids; // Llista de moviments valids

    void canviarTorn();
};

#endif 
