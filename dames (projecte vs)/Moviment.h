#include <iostream>
using namespace std;

#include "Posicio.h"

class Moviment
{
public:
	//CADA FITXA TE UNS MOVIMENT VALIDS, ELS MOVIMENTS HAN DE SER DE FITXA
	Moviment() : m_nCaptures(0) {}

	void setOrigen(const Posicio& o) { m_origen = o; }
	void setDesti(const Posicio& d) { m_desti = d; }
	void afegirCaptura(const Posicio& c);

	int getNMoviments() const { return nMovimentsValids; }
	Moviment getMoviment(int index) const { return m_movimentsValids[index]; }

	Posicio getOrigen() const { return m_origen; }
	Posicio getDesti() const { return m_desti; }
	int getNCaptures() const { return m_nCaptures; }
	Posicio getCaptures(int i) { return m_captures[i]; }
	//metode per compr ovar des de movimentValids, si el mov. que es vol 
	//bool esMovimentDiagonal() const;

	Posicio getPosicioCaptura() const; //retorna la posicio de la fitxa capturada

	void actualitzaMovimentsNN(int fila, int columna);
	void actualitzaMovimentsND(int fila, int columna);
	void actualitzaMovimentsBN(int fila, int columna);
	void actualitzaMovimentsBD(int fila, int columna);

	void inicialitza();

private:
	//nose si cal
	Posicio m_origen; //nomes volem guardar una captura per moviment
	Posicio m_desti;
	Posicio m_captures[12]; //max de captures en un moviment
	int m_nCaptures;

	Moviment m_movimentsValids[100];
	int nMovimentsValids;


};