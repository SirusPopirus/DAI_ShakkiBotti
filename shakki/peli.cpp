#include "peli.h"

Peli::Peli(int ihmisenVari)
{
    _koneenVari = 1 - ihmisenVari;
}

int Peli::getKoneenVari() {
    return _koneenVari;
}
