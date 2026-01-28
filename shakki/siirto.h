#pragma once
#include "ruutu.h"

// Ns. "forward declaration". Nyt Asema-luokassa voidaa esitellд Nappula-osoittimia ilman,
// ettд nappula.h -tiedostoa tдytyy includoida.
class Nappula; 


// Siirto kuvaa nappulan siirtymisen ruudusta toiseen, mukaanlukien erikoissiirrot
// (linnoitus ja ohestalyцnti).
class Siirto
{
public:
	Siirto(Ruutu, Ruutu);
	Siirto(){}
	Siirto(bool, bool); // Linnoitus lyhesti (K-siipi) tai pitkдsti (D-siipi?
	Ruutu getAlkuruutu();
	Ruutu getLoppuruutu();
	bool onkoLyhytLinna();
	bool onkoPitkälinna();
	Nappula* _miksikorotetaan = 0; 

private:
	Ruutu _alkuRuutu;
	Ruutu _loppuRuutu;
	bool _lyhytLinna;
	bool _pitkaLinna;
};