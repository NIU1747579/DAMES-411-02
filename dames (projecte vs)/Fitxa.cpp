#include "Fitxa.h"

void Fitxa::netejaMovimentsValids() 
{
	m_movimentsValids.clear();
}

void Fitxa::afegeixMovimentValid(const Moviment& moviment) 
{
	m_movimentsValids.push_back(moviment); // Afegeix al final del vector 
}

void Fitxa::visualitzaFitxa(float x, float y) const {

	if (tipus_Fitxa == TIPUS_NORMAL && color_Fitxa == COLOR_BLANC) {
		GraphicManager::getInstance()->drawSprite(GRAFIC_FITXA_BLANCA, x, y);
	}
	else if (tipus_Fitxa == TIPUS_NORMAL && color_Fitxa == COLOR_NEGRE) {
		GraphicManager::getInstance()->drawSprite(GRAFIC_FITXA_NEGRA, x, y);
	}
	else if (tipus_Fitxa == TIPUS_DAMA && color_Fitxa == COLOR_NEGRE) {
		GraphicManager::getInstance()->drawSprite(GRAFIC_DAMA_NEGRA, x, y);

	}
	else if (tipus_Fitxa == TIPUS_DAMA && color_Fitxa == COLOR_BLANC) {
		GraphicManager::getInstance()->drawSprite(GRAFIC_DAMA_BLANCA, x, y);

	}
}