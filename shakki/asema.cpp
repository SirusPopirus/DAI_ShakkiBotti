#include <iostream>
#include "asema.h"
#include "minmaxpaluu.h"
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

        // ohestalyönti: jos sotilas liikkuu diagonaaliin tyhjään ruutuun
        if (nappula && nappula->getKoodi() == VS && _siirtovuoro == 0) {
            if (ar == 4 && lr == 5 && as != ls && _lauta[lr][ls] == nullptr && kaksoisaskelSarakkeella == ls) {
                _lauta[4][ls] = nullptr;
            }
        }
        if (nappula && nappula->getKoodi() == MS && _siirtovuoro == 1) {
            if (ar == 3 && lr == 2 && as != ls && _lauta[lr][ls] == nullptr && kaksoisaskelSarakkeella == ls) {
                _lauta[3][ls] = nullptr;
            }
        }

        _lauta[ar][as] = nullptr;
        _lauta[lr][ls] = nappula;

        // kaksoisaskel
        if (nappula) {
            if (nappula->getKoodi() == VS && ar == 1 && lr == 3) {
                kaksoisaskelSarakkeella = as;
            }
            if (nappula->getKoodi() == MS && ar == 6 && lr == 4) {
                kaksoisaskelSarakkeella = as;
            }
        }

        if (nappula == vk) _onkoValkeaKuningasLiikkunut = true;
        if (nappula == mk) _onkoMustaKuningasLiikkunut = true;

        if (nappula == vt && ar == 0 && as == 0) _onkoValkeaDTliikkunut = true;
        if (nappula == vt && ar == 0 && as == 7) _onkoValkeaKTliikkunut = true;
        if (nappula == mt && ar == 7 && as == 0) _onkoMustaDTliikkunut = true;
        if (nappula == mt && ar == 7 && as == 7) _onkoMustaKTliikkunut = true;

        if (siirto->_miksikorotetaan != nullptr) {
            _lauta[lr][ls] = siirto->_miksikorotetaan;
        }
    }

    _siirtovuoro = 1 - _siirtovuoro;
}

int Asema::getSiirtovuoro() { return _siirtovuoro; }
void Asema::setSiirtovuoro(int vari) { _siirtovuoro = vari; }

bool Asema::getOnkoValkeaKuningasLiikkunut() { return _onkoValkeaKuningasLiikkunut; }
bool Asema::getOnkoMustaKuningasLiikkunut() { return _onkoMustaKuningasLiikkunut; }
bool Asema::getOnkoValkeaDTliikkunut() { return _onkoValkeaDTliikkunut; }
bool Asema::getOnkoValkeaKTliikkunut() { return _onkoValkeaKTliikkunut; }
bool Asema::getOnkoMustaDTliikkunut() { return _onkoMustaDTliikkunut; }
bool Asema::getOnkoMustaKTliikkunut() { return _onkoMustaKTliikkunut; }

double Asema::evaluoi() { return 0; }
double Asema::laskeNappuloidenArvo(int) { return 0; }
bool Asema::onkoAvausTaiKeskipeli(int) { return false; }
double Asema::nappuloitaKeskella(int) { return 0; }
double Asema::linjat(int) { return 0; }

MinMaxPaluu Asema::minimax(int syvyys)
{
    MinMaxPaluu p{};
    p._evaluointiArvo = 0;
    return p;
}
MinMaxPaluu Asema::maxi(int syvyys)
{
    MinMaxPaluu p{};
    p._evaluointiArvo = 0;
    return p;
}
MinMaxPaluu Asema::mini(int syvyys)
{
    MinMaxPaluu p{};
    p._evaluointiArvo = 0;
    return p;
}

void Asema::annaLinnoitusSiirrot(std::list<Siirto>& lista, int vari)
{
    int r = (vari == 0 ? 0 : 7);
    Nappula* k = (vari == 0 ? vk : mk);

    if (_lauta[r][4] != k) return;

    Ruutu kunRuutu(r, 4);
    if (onkoRuutuUhattu(&kunRuutu, 1 - vari)) return;

    bool kunLiikkunut = (vari == 0 ? _onkoValkeaKuningasLiikkunut : _onkoMustaKuningasLiikkunut);
    bool ktLiikkunut = (vari == 0 ? _onkoValkeaKTliikkunut : _onkoMustaKTliikkunut);
    bool dtLiikkunut = (vari == 0 ? _onkoValkeaDTliikkunut : _onkoMustaDTliikkunut);

    // lyhyt linna
    if (!kunLiikkunut && !ktLiikkunut) {
        if (_lauta[r][5] == nullptr && _lauta[r][6] == nullptr &&
            _lauta[r][7] == (vari == 0 ? vt : mt)) {
            Ruutu r5(r, 5), r6(r, 6);
            if (!onkoRuutuUhattu(&r5, 1 - vari) && !onkoRuutuUhattu(&r6, 1 - vari)) {
                lista.push_back(Siirto(true, false));
            }
        }
    }

    // pitkä linna
    if (!kunLiikkunut && !dtLiikkunut) {
        if (_lauta[r][1] == nullptr && _lauta[r][2] == nullptr && _lauta[r][3] == nullptr &&
            _lauta[r][0] == (vari == 0 ? vt : mt)) {
            Ruutu r3(r, 3), r2(r, 2);
            if (!onkoRuutuUhattu(&r3, 1 - vari) && !onkoRuutuUhattu(&r2, 1 - vari)) {
                lista.push_back(Siirto(false, true));
            }
        }
    }
}

void Asema::annaLaillisetSiirrot(std::list<Siirto>& lista)
{
    lista.clear();

    // 1) raakasiirrot
    for (int r = 0; r < 8; r++) {
        for (int s = 0; s < 8; s++) {
            Nappula* n = _lauta[r][s];
            if (n != nullptr && n->getVari() == _siirtovuoro) {
                Ruutu ruutu(r, s);
                n->annaSiirrot(lista, &ruutu, this, _siirtovuoro);
            }
        }
    }

    // 2) linnoitus
    annaLinnoitusSiirrot(lista, _siirtovuoro);

    // 3) kuninkaan ruutu
    Ruutu kuninkaanRuutu;
    for (int r = 0; r < 8; r++) {
        for (int s = 0; s < 8; s++) {
            if (_lauta[r][s] != nullptr) {
                if ((_siirtovuoro == 0 && _lauta[r][s] == vk) ||
                    (_siirtovuoro == 1 && _lauta[r][s] == mk)) {
                    kuninkaanRuutu = Ruutu(r, s);
                }
            }
        }
    }

    int vastustaja = 1 - _siirtovuoro;

    // 4) suodatus: poistetaan siirrot, jotka jättävät kuninkaan shakkiin
    for (auto it = lista.begin(); it != lista.end(); ) {

        if (it->onkoLyhytLinna() || it->onkoPitkalinna()) {
            ++it;
            continue;
        }

        Ruutu alku = it->getAlkuruutu();
        Ruutu loppu = it->getLoppuruutu();

        int ar = alku.getRivi();
        int as = alku.getSarake();
        int lr = loppu.getRivi();
        int ls = loppu.getSarake();

        Nappula* siirtyva = _lauta[ar][as];
        Nappula* syoty = _lauta[lr][ls];

        // perussiirto laudalle
        _lauta[ar][as] = nullptr;
        _lauta[lr][ls] = siirtyva;

        // --- OHESTALYÖNNIN SIMULAATIO ---
        bool enPassant = false;
        int eatenR = -1, eatenS = -1;
        Nappula* tempEP = nullptr;

        if (siirtyva && siirtyva->getKoodi() == VS && _siirtovuoro == 0) {
            if (ar == 4 && lr == 5 && as != ls && syoty == nullptr && ls == kaksoisaskelSarakkeella) {
                enPassant = true;
                eatenR = 4;
                eatenS = ls;
            }
        }
        if (siirtyva && siirtyva->getKoodi() == MS && _siirtovuoro == 1) {
            if (ar == 3 && lr == 2 && as != ls && syoty == nullptr && ls == kaksoisaskelSarakkeella) {
                enPassant = true;
                eatenR = 3;
                eatenS = ls;
            }
        }

        if (enPassant) {
            tempEP = _lauta[eatenR][eatenS];
            _lauta[eatenR][eatenS] = nullptr;
        }

        // kuninkaan testiruutu
        Ruutu testiruutu = kuninkaanRuutu;
        if (siirtyva == vk || siirtyva == mk) {
            testiruutu = loppu;
        }

        bool uhattu = onkoRuutuUhattu(&testiruutu, vastustaja);

        // palautetaan asema
        _lauta[ar][as] = siirtyva;
        _lauta[lr][ls] = syoty;
        if (enPassant) {
            _lauta[eatenR][eatenS] = tempEP;
        }

        if (uhattu) {
            it = lista.erase(it);
        }
        else {
            ++it;
        }
    }
}


bool Asema::onkoRuutuUhattu(Ruutu* ruutu, int vari)
{
    int r = ruutu->getRivi();
    int s = ruutu->getSarake();

    // 1) SOTILAAT (vari = hyökkääjän väri)
    // valkea sotilas lyö (r-1, s±1), musta (r+1, s±1), koska r kasvaa alhaalta ylöspäin
    int sr = (vari == 0 ? r - 1 : r + 1);
    if (sr >= 0 && sr < 8) {
        if (s - 1 >= 0 && _lauta[sr][s - 1] == (vari == 0 ? vs : ms)) return true;
        if (s + 1 < 8 && _lauta[sr][s + 1] == (vari == 0 ? vs : ms)) return true;
    }

    // 2) RATSUT
    int drN[8] = { 2,2,-2,-2,1,1,-1,-1 };
    int dsN[8] = { 1,-1,1,-1,2,-2,2,-2 };
    for (int i = 0; i < 8; i++) {
        int nr = r + drN[i], ns = s + dsN[i];
        if (nr >= 0 && nr < 8 && ns >= 0 && ns < 8) {
            if (_lauta[nr][ns] == (vari == 0 ? vr : mr)) return true;
        }
    }

    // 3) TORNI + DAAMI (suorat linjat)
    int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
    for (auto& d : dirs) {
        int nr = r + d[0], ns = s + d[1];
        while (nr >= 0 && nr < 8 && ns >= 0 && ns < 8) {
            if (_lauta[nr][ns] != nullptr) {
                if (_lauta[nr][ns] == (vari == 0 ? vt : mt) ||
                    _lauta[nr][ns] == (vari == 0 ? vd : md))
                    return true;
                break;
            }
            nr += d[0]; ns += d[1];
        }
    }

    // 4) LÄHETTI + DAAMI (diagonaalit)
    int d2[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
    for (auto& d : d2) {
        int nr = r + d[0], ns = s + d[1];
        while (nr >= 0 && nr < 8 && ns >= 0 && ns < 8) {
            if (_lauta[nr][ns] != nullptr) {
                if (_lauta[nr][ns] == (vari == 0 ? vl : ml) ||
                    _lauta[nr][ns] == (vari == 0 ? vd : md))
                    return true;
                break;
            }
            nr += d[0]; ns += d[1];
        }
    }

    // 5) KUNINGAS (viereiset ruudut)
    for (int drr = -1; drr <= 1; drr++) {
        for (int dss = -1; dss <= 1; dss++) {
            if (drr == 0 && dss == 0) continue;
            int nr = r + drr, ns = s + dss;
            if (nr >= 0 && nr < 8 && ns >= 0 && ns < 8) {
                if (_lauta[nr][ns] == (vari == 0 ? vk : mk)) return true;
            }
        }
    }

    return false;
}


void Asema::huolehdiKuninkaanShakeista(std::list<Siirto>&, int) {}
