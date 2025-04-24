#include "Fitxa.h"

void Fitxa::convertirADama()
{
	if (tipus_Fitxa == TIPUS_NORMAL)
		tipus_Fitxa = TIPUS_DAMA;
}
bool Fitxa::esContraria(const Fitxa& contrari)
{
	if (tipus_Fitxa == TIPUS_EMPTY || contrari.tipus_Fitxa == TIPUS_EMPTY)
	{
		return false;
	}
	return (color_Fitxa != contrari.color_Fitxa); //si son diferents
}

void Fitxa::calcularMovimentValids()
{

}