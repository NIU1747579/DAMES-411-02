#include "Fitxa.h"

void Fitxa::convertirADama()
{
	if (tipus_Fitxa == TIPUS_NORMAL)
		tipus_Fitxa = TIPUS_DAMA;
}
bool Fitxa::esContraria(const Fitxa& contrari) const
{
	if (esBuida() || contrari.esBuida()) return false;
	return (color_Fitxa != contrari.color_Fitxa); //si son diferents
}
