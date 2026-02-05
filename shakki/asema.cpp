#include <iostream>
#include "asema.h"
#include "minMaxPaluu.h"
#include "nappula.h"
#include "ruutu.h"
using namespace std;

Nappula* Asema::vk = new Kuningas(L"\u2654", 0, VK);
Nappula* Asema::vd = new Daami(L"\u2655", 0, VD);
Nappula* Asema::vt = new Torni(L"\u2656", 0, VT);
Nappula* Asema::vl = new Lahetti(L"\u2657", 0, VL);
Nappula* Asema::vr = new Ratsu(L"\u2658", 0, VR);
Nappula* Asema::vs = new Sotilas(L"\u2659", 0, VS);

Nappula* Asema::mk = new Kuningas(L"\u265A", 1, MK);
Nappula* Asema::md = new Daami(L"\u265B", 1, MD);
Nappula* Asema::mt = new Torni(L"\u265C", 1, MT);
Nappula* Asema::ml = new Lahetti(L"\u265D", 1, ML);
Nappula* Asema::mr = new Ratsu(L"\u265E", 1, MR);
Nappula* Asema::ms = new Sotilas(L"\u265F", 1, MS);


Asema::Asema()
{
    for (int i = 0; i < 8; i++)
        for (int j = 0; j < 8; j++)
            _lauta[i][j] = nullptr;

    _lauta[0][0] = vt; _lauta[0][1] = vr; _lauta[0][2] = vl; _lauta[0][3] = vd;
    _lauta[0][4] = vk; _lauta[0][5] = vl; _lauta[0][6] = vr; _lauta[0][7] = vt;
    for (int i = 0; i < 8; i++) _lauta[1][i] = vs;

    _lauta[7][0] = mt; _lauta[7][1] = mr; _lauta[7][2] = ml; _lauta[7][3] = md;
    _lauta[7][4] = mk; _lauta[7][5] = ml; _lauta[7][6] = mr; _lauta[7][7] = mt;
    for (int i = 0; i < 8; i++) _lauta[6][i] = ms;

    _siirtovuoro = 0;
    _onkoValkeaKuningasLiikkunut = false;
    _onkoMustaKuningasLiikkunut = false;
    _onkoValkeaDTliikkunut = false;
    _onkoValkeaKTliikkunut = false;
    _onkoMustaDTliikkunut = false;
    _onkoMustaKTliikkunut = false;

    kaksoisaskelSarakkeella = -1;
}



void Asema::paivitaAsema(Siirto* siirto)
{
    kaksoisaskelSarakkeella = -1;

    if (siirto->onkoLyhytLinna() || siirto->onkoPitkalinna()) {
        if (_siirtovuoro == 0) {
            if (siirto->onkoLyhytLinna()) {
                _lauta[0][6] = _lauta[0][4];
                _lauta[0][4] = nullptr;
                _lauta[0][5] = _lauta[0][7];
                _lauta[0][7] = nullptr;

                _onkoValkeaKuningasLiikkunut = true;
                _onkoValkeaKTliikkunut = true;
            }
            else {
                _lauta[0][2] = _lauta[0][4];
                _lauta[0][4] = nullptr;
                _lauta[0][3] = _lauta[0][0];
                _lauta[0][0] = nullptr;

                _onkoValkeaKuningasLiikkunut = true;
                _onkoValkeaDTliikkunut = true;
            }
        }
        else {
            if (siirto->onkoLyhytLinna()) {
                _lauta[7][6] = _lauta[7][4];
                _lauta[7][4] = nullptr;
                _lauta[7][5] = _lauta[7][7];
                _lauta[7][7] = nullptr;

                _onkoMustaKuningasLiikkunut = true;
                _onkoMustaKTliikkunut = true;
            }
            else {
                _lauta[7][2] = _lauta[7][4];
                _lauta[7][4] = nullptr;
                _lauta[7][3] = _lauta[7][0];
                _lauta[7][0] = nullptr;

                _onkoMustaKuningasLiikkunut = true;
                _onkoMustaDTliikkunut = true;
            }
        }
    }
    else {
        Ruutu alku = siirto->getAlkuruutu();
        Ruutu loppu = siirto->getLoppuruutu();

        int ar = alku.getRivi();
        int as = alku.getSarake();
        int lr = loppu.getRivi();
        int ls = loppu.getSarake();

        Nappula* nappula = _lauta[ar][as];
        _lauta[ar][as] = nullptr;
        _lauta[lr][ls] = nappula;

        if (nappula == vk) {
            _onkoValkeaKuningasLiikkunut = true;
        }
        if (nappula == mk) {
            _onkoMustaKuningasLiikkunut = true;
        }

        if (nappula == vt && ar == 0 && as == 0) {
            _onkoValkeaDTliikkunut = true;
        }
        if (nappula == vt && ar == 0 && as == 7) {
            _onkoValkeaKTliikkunut = true;
        }
        if (nappula == mt && ar == 7 && as == 0) {
            _onkoMustaDTliikkunut = true;
        }
        if (nappula == mt && ar == 7 && as == 7) {
            _onkoMustaKTliikkunut = true;
        }

    }
    _siirtovuoro = 1 - _siirtovuoro;
}


int Asema::getSiirtovuoro()
{
    return _siirtovuoro;
}

void Asema::setSiirtovuoro(int vari)
{
    _siirtovuoro = vari;
}

bool Asema::getOnkoValkeaKuningasLiikkunut()
{
    return _onkoValkeaKuningasLiikkunut;
}

bool Asema::getOnkoMustaKuningasLiikkunut()
{
    return _onkoMustaKuningasLiikkunut;
}

bool Asema::getOnkoValkeaDTliikkunut()
{
    return _onkoValkeaDTliikkunut;
}

bool Asema::getOnkoValkeaKTliikkunut()
{
    return _onkoValkeaKTliikkunut;
}

bool Asema::getOnkoMustaDTliikkunut()
{
    return _onkoMustaDTliikkunut;
}

bool Asema::getOnkoMustaKTliikkunut()
{
    return _onkoMustaKTliikkunut;
}

double Asema::evaluoi()
{
    return 0;
}

double Asema::laskeNappuloidenArvo(int vari)
{
    return 0;
}

bool Asema::onkoAvausTaiKeskipeli(int vari)
{
    return 0;
}

double Asema::nappuloitaKeskella(int vari)
{
    return 0;
}

double Asema::linjat(int vari)
{
    return 0;
}

MinMaxPaluu Asema::minimax(int syvyys)
{
    MinMaxPaluu paluuarvo;
    return paluuarvo;
}

MinMaxPaluu Asema::maxi(int syvyys)
{
    MinMaxPaluu paluu;
    return paluu;
}

MinMaxPaluu Asema::mini(int syvyys)
{
    MinMaxPaluu paluu;
    return paluu;
}

bool Asema::onkoRuutuUhattu(Ruutu* ruutu, int vastustajanVari)
{
    return false;
}

void Asema::huolehdiKuninkaanShakeista(std::list<Siirto>& lista, int vari)
{
}

void Asema::annaLaillisetSiirrot(std::list<Siirto>& lista)
{
    lista.clear();

    for (int r = 0; r < 8; r++) {
        for (int s = 0; s < 8; s++) {
            Nappula* n = _lauta[r][s];
            if (n != nullptr && n->getVari() == _siirtovuoro) {
                Ruutu ruutu(r, s);
                n->annaSiirrot(lista, &ruutu, this, _siirtovuoro);
            }
        }
    }
}
