//
//  main.cpp
//
//  Copyright � 2018 Compiled Creations Limited. All rights reserved.
//

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__) || defined  (_WIN64)

#include <iostream>
//Definicio necesaria per poder incloure la llibreria i que trobi el main
#define SDL_MAIN_HANDLED
#include <windows.h>
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#include <conio.h>      /* getch */ 

#elif __APPLE__
//Llibreria grafica
#include "../Graphic Lib/libreria.h"
#include "../Graphic Lib/NFont/NFont.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdocumentation"
#include <SDL2/SDL.h>
#pragma clang diagnostic pop

#endif

#include "./joc.hpp"
#include "./info_joc.hpp"

ModeJoc seleccionarModeJoc(Screen& pantalla, bool& salirDelPrograma) {
    ModeJoc modeSeleccionat = MODE_JOC_NORMAL; // Valor por defecto
    bool seleccionat = false;

    // No molt clar les mides
    const int ampladaClic = 300;
    const int altoBoton = 60;
    const int posXBoton = (TAMANY_PANTALLA_X - ampladaClic) / 2; // Centrat horit
    const int posYTitulo = 150;
    const int posYBotonNormal = 300;
    const int posYBotonReplay = 400;
    const int textOffsetY = (altoBoton / 2) - 15;

    while (!seleccionat && !salirDelPrograma) {
        pantalla.processEvents();

        // Surt del programa si es precioma ESC
        if (Keyboard_GetKeyTrg(KEYBOARD_ESCAPE)) {
            salirDelPrograma = true;
            break;
        }

        GraphicManager::getInstance()->drawSprite(GRAFIC_FONS, 0, 0);
        string titol = "SELECCIONA MODE DE JOC";
        GraphicManager::getInstance()->drawFont( FONT_WHITE_30,(TAMANY_PANTALLA_X / 2) - 150, posYTitulo, 1.0, titol);

        // Obtenim l'estat del ratoli
        int mouseX = Mouse_getX();
        int mouseY = Mouse_getY();
        bool mouseClick = Mouse_getBtnLeft();

        // Per detectar el ratoli a sobre del boto (NORMAL)
        bool mouseSobreNormal = (mouseX >= posXBoton && mouseX <= posXBoton + ampladaClic &&
            mouseY >= posYBotonNormal && mouseY <= posYBotonNormal + altoBoton);

        if (mouseSobreNormal) {
            GraphicManager::getInstance()->drawFont(FONT_GREEN_30, posXBoton + ampladaClic / 2 - 70,
                posYBotonNormal + textOffsetY, 1.0, "MODO NORMAL");
        }
        else {
            GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posXBoton + ampladaClic / 2 - 70,
                posYBotonNormal + textOffsetY, 1.0, "MODO NORMAL");
        }

        // Per detectar el ratoli a sobre del boto (REPLAY)
        bool mouseSobreReplay = (mouseX >= posXBoton && mouseX <= posXBoton + ampladaClic &&
            mouseY >= posYBotonReplay && mouseY <= posYBotonReplay + altoBoton);

        if (mouseSobreReplay) {
            GraphicManager::getInstance()->drawFont(FONT_GREEN_30, posXBoton + ampladaClic / 2 - 70,
                posYBotonReplay + textOffsetY, 1.0, "MODO REPLAY");
        }
        else {
            GraphicManager::getInstance()->drawFont(FONT_WHITE_30, posXBoton + ampladaClic / 2 - 70,
                posYBotonReplay + textOffsetY, 1.0, "MODO REPLAY");
        }

        if (mouseClick) {
            if (mouseSobreNormal) {
                modeSeleccionat = MODE_JOC_NORMAL;
                seleccionat = true;
            }
            else if (mouseSobreReplay) {
                modeSeleccionat = MODE_JOC_REPLAY;
                seleccionat = true;
            }
        }

        pantalla.update();
    }

    return modeSeleccionat;
}

int main(int argc, const char* argv[])
{
    SDL_SetMainReady();
    SDL_Init(SDL_INIT_VIDEO);

    Screen pantalla(TAMANY_PANTALLA_X, TAMANY_PANTALLA_Y);
    pantalla.show();

    bool salirDelPrograma = false;

    while (!salirDelPrograma)
    {
        // Mostrar menú de selección de modo
        ModeJoc mode = seleccionarModeJoc(pantalla, salirDelPrograma);

        // Si salirDelPrograma es true, terminar el programa
        if (salirDelPrograma) {
            break;
        }

        // Inicializar el juego con el modo seleccionado
        Joc joc;
        joc.inicialitza(mode, "data/Games/tauler_inicial.txt", "data/Games/moviments.txt");

        bool volverAlMenu = false;
        do
        {
            pantalla.processEvents();
            bool mouseStatus = Mouse_getBtnLeft();
            int mousePosX = Mouse_getX();
            int mousePosY = Mouse_getY();
            bool final = joc.actualitza(mousePosX, mousePosY, mouseStatus);

            // Verificar si se presionó ESCAPE durante el juego
            if (Keyboard_GetKeyTrg(KEYBOARD_ESCAPE))
            {
                volverAlMenu = true;
            }

            pantalla.update();
        } while (!volverAlMenu && !Keyboard_GetKeyTrg(KEYBOARD_ESCAPE));

        joc.finalitza();
    }

    SDL_Quit();
    return 0;
}
