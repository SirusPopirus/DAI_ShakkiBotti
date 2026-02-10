#pragma once
#include "asema.h"
#include "nappula.h"
#include "peli.h"
#include "siirto.h"

class Kayttoliittyma
{
public:
    void aseta_asema(Asema* asema) { this->_asema = asema; }
    void piirraLauta();
    Siirto annaVastustajanSiirto();
    int kysyVastustajanVari();

    static Kayttoliittyma* getInstance();

private:
    Asema* _asema;
    static Kayttoliittyma* instance;

    Kayttoliittyma() {}
    Kayttoliittyma(Asema* asema) { this->_asema = asema; }
};