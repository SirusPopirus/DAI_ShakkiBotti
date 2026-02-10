#pragma once
#include <list>
#include <string>
#include "minmaxpaluu.h"
#include "siirto.h"

class Nappula;

class Asema
{
public:
    Nappula* _lauta[8][8];

    static Nappula* vk, * vd, * vt, * vl, * vr, * vs;
    static Nappula* mk, * md, * mt, * ml, * mr, * ms;

    int kaksoisaskelSarakkeella = -1;

    Asema();
    void paivitaAsema(Siirto*);
    double evaluoi();
    MinMaxPaluu maxi(int syvyys);
    MinMaxPaluu mini(int syvyys);
    MinMaxPaluu minimax(int syvyys);
    void annaLaillisetSiirrot(std::list<Siirto>& lista);
    int getSiirtovuoro();
    void setSiirtovuoro(int vari);
    bool getOnkoValkeaKuningasLiikkunut();
    bool getOnkoMustaKuningasLiikkunut();
    bool getOnkoValkeaDTliikkunut();
    bool getOnkoValkeaKTliikkunut();
    bool getOnkoMustaDTliikkunut();
    bool getOnkoMustaKTliikkunut();

private:
    int _siirtovuoro;
    bool _onkoValkeaKuningasLiikkunut;
    bool _onkoMustaKuningasLiikkunut;
    bool _onkoValkeaDTliikkunut;
    bool _onkoValkeaKTliikkunut;
    bool _onkoMustaDTliikkunut;
    bool _onkoMustaKTliikkunut;

    double laskeNappuloidenArvo(int);
    bool onkoAvausTaiKeskipeli(int);
    double nappuloitaKeskella(int);
    double linjat(int);
    bool onkoRuutuUhattu(Ruutu*, int vastustajanVari);
    void annaLinnoitusSiirrot(std::list<Siirto>& lista, int vari);
    void huolehdiKuninkaanShakeista(std::list<Siirto>& lista, int vari);
};