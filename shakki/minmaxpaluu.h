#pragma once
#include "siirto.h"


// luokka, jonka avulla saadaan palautettua minmax:ssa sek� siirto-olio ett� evaluointifunktion arvo
// Struct ajaisi saman asian. Kun ei rakenneta gettereit� ja settereit�, niin ei tarvita toteutus .cpp tiedostoa
class MinMaxPaluu {
public:
	double _evaluointiArvo;
	Siirto _parasSiirto;
};