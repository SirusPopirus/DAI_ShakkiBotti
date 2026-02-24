#include <iostream>
#include "asema.h"
#include "minmaxpaluu.h"
#include "nappula.h"
#include "ruutu.h"
#include <cmath>
#include <vector>
#include <algorithm>
using namespace std;

Nappula* Asema::vk = new Kuningas(L"\u2654",0, VK);
Nappula* Asema::vd = new Daami(L"\u2655",0, VD);
Nappula* Asema::vt = new Torni(L"\u2656",0, VT);
Nappula* Asema::vl = new Lahetti(L"\u2657",0, VL);
Nappula* Asema::vr = new Ratsu(L"\u2658",0, VR);
Nappula* Asema::vs = new Sotilas(L"\u2659",0, VS);

Nappula* Asema::mk = new Kuningas(L"\u265A",1, MK);
Nappula* Asema::md = new Daami(L"\u265B",1, MD);
Nappula* Asema::mt = new Torni(L"\u265C",1, MT);
Nappula* Asema::ml = new Lahetti(L"\u265D",1, ML);
Nappula* Asema::mr = new Ratsu(L"\u265E",1, MR);
Nappula* Asema::ms = new Sotilas(L"\u265F",1, MS);

Asema::Asema()
{
 for (int i =0; i <8; i++)
 for (int j =0; j <8; j++)
 _lauta[i][j] = nullptr;

 _lauta[0][0] = vt; _lauta[0][1] = vr; _lauta[0][2] = vl; _lauta[0][3] = vd;
 _lauta[0][4] = vk; _lauta[0][5] = vl; _lauta[0][6] = vr; _lauta[0][7] = vt;
 for (int i =0; i <8; i++) _lauta[1][i] = vs;

 _lauta[7][0] = mt; _lauta[7][1] = mr; _lauta[7][2] = ml; _lauta[7][3] = md;
 _lauta[7][4] = mk; _lauta[7][5] = ml; _lauta[7][6] = mr; _lauta[7][7] = mt;
 for (int i =0; i <8; i++) _lauta[6][i] = ms;

 _siirtovuoro =0;
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
 // NOTE: Do NOT reset kaksoisaskelSarakkeella here — it must remain available
 // for the opponent's immediate move to allow en passant capture.

 // --- LINNOITUS ---
 if (siirto->onkoLyhytLinna() || siirto->onkoPitkalinna()) {

 int r = (_siirtovuoro ==0 ?0 :7);

 if (siirto->onkoLyhytLinna()) {
 // Kuningas e → g
 _lauta[r][6] = _lauta[r][4];
 _lauta[r][4] = nullptr;

 // Torni h → f
 _lauta[r][5] = _lauta[r][7];
 _lauta[r][7] = nullptr;
 }
 else {
 // Kuningas e → c
 _lauta[r][2] = _lauta[r][4];
 _lauta[r][4] = nullptr;

 // Torni a → d
 _lauta[r][3] = _lauta[r][0];
 _lauta[r][0] = nullptr;
 }

 if (_siirtovuoro ==0) {
 _onkoValkeaKuningasLiikkunut = true;
 if (siirto->onkoLyhytLinna()) _onkoValkeaKTliikkunut = true;
 else _onkoValkeaDTliikkunut = true;
 }
 else {
 _onkoMustaKuningasLiikkunut = true;
 if (siirto->onkoLyhytLinna()) _onkoMustaKTliikkunut = true;
 else _onkoMustaDTliikkunut = true;
 }

 // After any move by the opponent the previous double-step opportunity expires
 // After a castle the previous double-step opportunity expires
 kaksoisaskelSarakkeella = -1;

 _siirtovuoro =1 - _siirtovuoro;
 return;
 }

 // --- NORMAALI SIIRTO ---
 Ruutu alku = siirto->getAlkuruutu();
 Ruutu loppu = siirto->getLoppuruutu();

 int ar = alku.getRivi();
 int as = alku.getSarake();
 int lr = loppu.getRivi();
 int ls = loppu.getSarake();

 Nappula* nappula = _lauta[ar][as];
 Nappula* syoty = _lauta[lr][ls];

 // --- OHESTALYÖNTI ---
 bool onValkea = (nappula->getVari() ==0);
 bool onMusta = (nappula->getVari() ==1);

 // valkea lyö ohesta
 if (onValkea &&
 nappula->getKoodi() == VS &&
 ar ==4 && lr ==5 && as != ls &&
 syoty == nullptr &&
 kaksoisaskelSarakkeella == ls)
 {
 _lauta[4][ls] = nullptr; // ← syötävä musta sotilas
 }

 // musta lyö ohesta
 if (onMusta &&
 nappula->getKoodi() == MS &&
 ar ==3 && lr ==2 && as != ls &&
 syoty == nullptr &&
 kaksoisaskelSarakkeella == ls)
 {
 _lauta[3][ls] = nullptr; // ← syötävä valkea sotilas
 }


 // --- SIIRRETÄÄN NAPPULA ---
 _lauta[ar][as] = nullptr;
 _lauta[lr][ls] = nappula;

 // --- KAKSOISASKEL ---
 bool oliKaksoisaskel = false;
 if (nappula->getKoodi() == VS && ar ==1 && lr ==3) {
 kaksoisaskelSarakkeella = as;
 oliKaksoisaskel = true;
 }

 if (nappula->getKoodi() == MS && ar ==6 && lr ==4) {
 kaksoisaskelSarakkeella = as;
 oliKaksoisaskel = true;
 }

 // jos nykyinen siirto ei ollut kaksoisaskel, edellinen mahdollisuus raukeaa
 if (!oliKaksoisaskel) kaksoisaskelSarakkeella = -1;

 // --- KUNINGAS / TORNI LIIKKUNUT ---
 if (nappula == vk) _onkoValkeaKuningasLiikkunut = true;
 if (nappula == mk) _onkoMustaKuningasLiikkunut = true;

 if (nappula == vt && ar ==0 && as ==0) _onkoValkeaDTliikkunut = true;
 if (nappula == vt && ar ==0 && as ==7) _onkoValkeaKTliikkunut = true;
 if (nappula == mt && ar ==7 && as ==0) _onkoMustaDTliikkunut = true;
 if (nappula == mt && ar ==7 && as ==7) _onkoMustaKTliikkunut = true;

 // --- KOROTUS ---
 if (siirto->_miksikorotetaan != nullptr) {

 int vari = nappula->getVari();
 int koodi = siirto->_miksikorotetaan->getKoodi();
 std::wstring uni = siirto->_miksikorotetaan->getUnicode();

 if (koodi == VD || koodi == MD)
 _lauta[lr][ls] = new Daami(uni, vari, koodi);

 else if (koodi == VT || koodi == MT)
 _lauta[lr][ls] = new Torni(uni, vari, koodi);

 else if (koodi == VL || koodi == ML)
 _lauta[lr][ls] = new Lahetti(uni, vari, koodi);

 else if (koodi == VR || koodi == MR)
 _lauta[lr][ls] = new Ratsu(uni, vari, koodi);
 }

 _siirtovuoro =1 - _siirtovuoro;
}


int Asema::getSiirtovuoro() { return _siirtovuoro; }
void Asema::setSiirtovuoro(int vari) { _siirtovuoro = vari; }

bool Asema::getOnkoValkeaKuningasLiikkunut() { return _onkoValkeaKuningasLiikkunut; }
bool Asema::getOnkoMustaKuningasLiikkunut() { return _onkoMustaKuningasLiikkunut; }
bool Asema::getOnkoValkeaDTliikkunut() { return _onkoValkeaDTliikkunut; }
bool Asema::getOnkoValkeaKTliikkunut() { return _onkoValkeaKTliikkunut; }
bool Asema::getOnkoMustaDTliikkunut() { return _onkoMustaDTliikkunut; }
bool Asema::getOnkoMustaKTliikkunut() { return _onkoMustaKTliikkunut; }

// --- Evaluointifunktio ja apufunktiot ---

double Asema::laskeNappuloidenArvo(int /*unused*/)
{
 // Returns white minus black in pawn units
 double val =0.0;
 for (int r =0; r <8; r++) {
 for (int s =0; s <8; s++) {
 Nappula* n = _lauta[r][s];
 if (!n) continue;
 double v =0.0;
 int k = n->getKoodi();
 if (k == VD || k == MD) v =9.0;
 else if (k == VT || k == MT) v =5.0;
 else if (k == VL || k == ML) v =3.25;
 else if (k == VR || k == MR) v =3.0;
 else if (k == VS || k == MS) v =1.0;

 if (n->getVari() ==0) val += v; else val -= v;
 }
 }
 return val;
}

bool Asema::onkoAvausTaiKeskipeli(int /*unused*/)
{
 // Simple phase detection: count non-pawn material (absolute value)
 double nonPawn =0.0;
 for (int r =0; r <8; r++) {
 for (int s =0; s <8; s++) {
 Nappula* n = _lauta[r][s];
 if (!n) continue;
 int k = n->getKoodi();
 if (k == VS || k == MS) continue;
 if (k == VD || k == MD) nonPawn +=9.0;
 else if (k == VT || k == MT) nonPawn +=5.0;
 else if (k == VL || k == ML) nonPawn +=3.25;
 else if (k == VR || k == MR) nonPawn +=3.0;
 }
 }
 // If there is a lot of material on board -> opening/midgame
 return nonPawn >10.0; // threshold chosen heuristically
}

double Asema::nappuloitaKeskella(int vari)
{
 // Reward pieces on central4x4 squares (ranks/files2..5)
 double cnt =0.0;
 for (int r =2; r <=5; r++) {
 for (int s =2; s <=5; s++) {
 Nappula* n = _lauta[r][s];
 if (!n) continue;
 if (n->getVari() == vari) {
 int k = n->getKoodi();
 // heavier pieces matter more in center
 double weight =0.5;
 if (k == VD || k == MD) weight =1.0;
 else if (k == VT || k == MT) weight =0.8;
 else if (k == VL || k == ML || k == VR || k == MR) weight =0.7;
 else if (k == VS || k == MS) weight =0.4;
 cnt += weight;
 }
 }
 }
 return cnt;
}

double Asema::linjat(int vari)
{
 // Bonus for rooks/queens on open files/ranks and bishops on open diagonals
 double bonus =0.0;
 for (int r =0; r <8; r++) {
 for (int s =0; s <8; s++) {
 Nappula* n = _lauta[r][s];
 if (!n) continue;
 if (n->getVari() != vari) continue;
 int k = n->getKoodi();
 if (k == VT || k == MT || k == VD || k == MD) {
 // check rank open
 bool rankOpen = true;
 for (int ss =0; ss <8; ss++) {
 if (ss == s) continue;
 if (_lauta[r][ss] != nullptr && _lauta[r][ss]->getKoodi() == VS) { rankOpen = false; break; }
 if (_lauta[r][ss] != nullptr && _lauta[r][ss]->getKoodi() == MS) { rankOpen = false; break; }
 }
 bool fileOpen = true;
 for (int rr =0; rr <8; rr++) {
 if (rr == r) continue;
 if (_lauta[rr][s] != nullptr && _lauta[rr][s]->getKoodi() == VS) { fileOpen = false; break; }
 if (_lauta[rr][s] != nullptr && _lauta[rr][s]->getKoodi() == MS) { fileOpen = false; break; }
 }
 if (rankOpen) bonus +=0.6;
 if (fileOpen) bonus +=0.8;
 }
 if (k == VL || k == ML) {
 // check diagonals for pawns blocking
 int d1r = r+1, d1s = s+1; bool d1open = true;
 while (d1r <8 && d1s <8) { if (_lauta[d1r][d1s] != nullptr && (_lauta[d1r][d1s]->getKoodi() == VS || _lauta[d1r][d1s]->getKoodi() == MS)) { d1open = false; break; } d1r++; d1s++; }
 int d2r = r+1, d2s = s-1; bool d2open = true;
 while (d2r <8 && d2s >=0) { if (_lauta[d2r][d2s] != nullptr && (_lauta[d2r][d2s]->getKoodi() == VS || _lauta[d2r][d2s]->getKoodi() == MS)) { d2open = false; break; } d2r++; d2s--; }
 int d3r = r-1, d3s = s+1; bool d3open = true;
 while (d3r >=0 && d3s <8) { if (_lauta[d3r][d3s] != nullptr && (_lauta[d3r][d3s]->getKoodi() == VS || _lauta[d3r][d3s]->getKoodi() == MS)) { d3open = false; break; } d3r--; d3s++; }
 int d4r = r-1, d4s = s-1; bool d4open = true;
 while (d4r >=0 && d4s >=0) { if (_lauta[d4r][d4s] != nullptr && (_lauta[d4r][d4s]->getKoodi() == VS || _lauta[d4r][d4s]->getKoodi() == MS)) { d4open = false; break; } d4r--; d4s--; }
 if (d1open || d2open || d3open || d4open) bonus +=0.5;
 }
 }
 }
 return bonus;
}


double Asema::evaluoi()
{
 // Base material (white positive)
 double material = laskeNappuloidenArvo(0);

 // Center control
 double centerWhite = nappuloitaKeskella(0);
 double centerBlack = nappuloitaKeskella(1);
 double centerAdv = centerWhite - centerBlack;

 // Open lines and diagonals
 double linesWhite = linjat(0);
 double linesBlack = linjat(1);
 double linesAdv = linesWhite - linesBlack;

 // King safety: small bonus if king hasn't moved (castle potential)
 double kingSafety =0.0;
 if (!_onkoValkeaKuningasLiikkunut) kingSafety +=0.3;
 if (!_onkoMustaKuningasLiikkunut) kingSafety -=0.3;

 // Combine: material primary, others are small modifiers
 double score = material
 +0.12 * centerAdv
 +0.08 * linesAdv
 + kingSafety;

 return score;
}


MinMaxPaluu Asema::minimax(int syvyys)
{
 // choose depending on side to move
 if (_siirtovuoro ==0) return maxi(syvyys);
 return mini(syvyys);
}

MinMaxPaluu Asema::maxi(int syvyys)
{
 MinMaxPaluu best;
 if (syvyys <=0) {
 best._evaluointiArvo = evaluoi();
 return best;
 }

 list<Siirto> moves;
 annaLaillisetSiirrot(moves);
 if (moves.empty()) {
 // checkmate or stalemate for white to move
 // find white king
 Ruutu kunRuutu;
 for (int r =0; r <8; ++r) for (int s =0; s <8; ++s) if (_lauta[r][s] == vk) kunRuutu = Ruutu(r, s);
 int vast =1 - _siirtovuoro;
 bool uhattu = onkoRuutuUhattu(&kunRuutu, vast);
 if (uhattu) best._evaluointiArvo = -10000.0; else best._evaluointiArvo =0.0;
 return best;
 }

 double bestVal = -1e9;
 Siirto bestMove;

 // save state
 Nappula* saved[8][8];
 for (int r =0; r <8; ++r) for (int s =0; s <8; ++s) saved[r][s] = _lauta[r][s];
 int savedSiirtovuoro = _siirtovuoro;
 int savedKaksois = kaksoisaskelSarakkeella;
 bool sv_vk = _onkoValkeaKuningasLiikkunut;
 bool sv_mk = _onkoMustaKuningasLiikkunut;
 bool sv_vdt = _onkoValkeaDTliikkunut;
 bool sv_vkt = _onkoValkeaKTliikkunut;
 bool sv_mdt = _onkoMustaDTliikkunut;
 bool sv_mkt = _onkoMustaKTliikkunut;

 for (auto &m : moves) {
 // apply move
 paivitaAsema(&m);
 // recurse
 MinMaxPaluu reply = mini(syvyys -1);
 double val = reply._evaluointiArvo;
 if (val > bestVal) { bestVal = val; bestMove = m; }
 // restore state (note: this does not delete newly allocated promotion pieces)
 for (int r =0; r <8; ++r) for (int s =0; s <8; ++s) _lauta[r][s] = saved[r][s];
 _siirtovuoro = savedSiirtovuoro;
 kaksoisaskelSarakkeella = savedKaksois;
 _onkoValkeaKuningasLiikkunut = sv_vk;
 _onkoMustaKuningasLiikkunut = sv_mk;
 _onkoValkeaDTliikkunut = sv_vdt;
 _onkoValkeaKTliikkunut = sv_vkt;
 _onkoMustaDTliikkunut = sv_mdt;
 _onkoMustaKTliikkunut = sv_mkt;
 }

 best._evaluointiArvo = bestVal;
 best._parasSiirto = bestMove;
 return best;
}

MinMaxPaluu Asema::mini(int syvyys)
{
 MinMaxPaluu best;
 if (syvyys <=0) {
 best._evaluointiArvo = evaluoi();
 return best;
 }

 list<Siirto> moves;
 annaLaillisetSiirrot(moves);
 if (moves.empty()) {
 // checkmate or stalemate for black to move
 Ruutu kunRuutu;
 for (int r =0; r <8; ++r) for (int s =0; s <8; ++s) if (_lauta[r][s] == mk) kunRuutu = Ruutu(r, s);
 int vast =1 - _siirtovuoro;
 bool uhattu = onkoRuutuUhattu(&kunRuutu, vast);
 if (uhattu) best._evaluointiArvo =10000.0; else best._evaluointiArvo =0.0;
 return best;
 }

 double bestVal =1e9;
 Siirto bestMove;

 // save state
 Nappula* saved[8][8];
 for (int r =0; r <8; ++r) for (int s =0; s <8; ++s) saved[r][s] = _lauta[r][s];
 int savedSiirtovuoro = _siirtovuoro;
 int savedKaksois = kaksoisaskelSarakkeella;
 bool sv_vk = _onkoValkeaKuningasLiikkunut;
 bool sv_mk = _onkoMustaKuningasLiikkunut;
 bool sv_vdt = _onkoValkeaDTliikkunut;
 bool sv_vkt = _onkoValkeaKTliikkunut;
 bool sv_mdt = _onkoMustaDTliikkunut;
 bool sv_mkt = _onkoMustaKTliikkunut;

 for (auto &m : moves) {
 paivitaAsema(&m);
 MinMaxPaluu reply = maxi(syvyys -1);
 double val = reply._evaluointiArvo;
 if (val < bestVal) { bestVal = val; bestMove = m; }
 // restore
 for (int r =0; r <8; ++r) for (int s =0; s <8; ++s) _lauta[r][s] = saved[r][s];
 _siirtovuoro = savedSiirtovuoro;
 kaksoisaskelSarakkeella = savedKaksois;
 _onkoValkeaKuningasLiikkunut = sv_vk;
 _onkoMustaKuningasLiikkunut = sv_mk;
 _onkoValkeaDTliikkunut = sv_vdt;
 _onkoValkeaKTliikkunut = sv_vkt;
 _onkoMustaDTliikkunut = sv_mdt;
 _onkoMustaKTliikkunut = sv_mkt;
 }

 best._evaluointiArvo = bestVal;
 best._parasSiirto = bestMove;
 return best;
}


void Asema::annaLinnoitusSiirrot(std::list<Siirto>& lista, int vari)
{
 int r = (vari ==0 ?0 :7);
 Nappula* k = (vari ==0 ? vk : mk);

 if (_lauta[r][4] != k) return;

 Ruutu kunRuutu(r,4);
 if (onkoRuutuUhattu(&kunRuutu,1 - vari)) return;

 bool kunLiikkunut = (vari ==0 ? _onkoValkeaKuningasLiikkunut : _onkoMustaKuningasLiikkunut);
 bool ktLiikkunut = (vari ==0 ? _onkoValkeaKTliikkunut : _onkoMustaKTliikkunut);
 bool dtLiikkunut = (vari ==0 ? _onkoValkeaDTliikkunut : _onkoMustaDTliikkunut);

 // lyhyt linna
 if (!kunLiikkunut && !ktLiikkunut) {
 if (_lauta[r][5] == nullptr && _lauta[r][6] == nullptr &&
 _lauta[r][7] == (vari ==0 ? vt : mt)) {
 Ruutu r5(r,5), r6(r,6);
 if (!onkoRuutuUhattu(&r5,1 - vari) && !onkoRuutuUhattu(&r6,1 - vari)) {
 lista.push_back(Siirto(true, false));
 }
 }
 }

 // pitkä linna
 if (!kunLiikkunut && !dtLiikkunut) {
 if (_lauta[r][1] == nullptr && _lauta[r][2] == nullptr && _lauta[r][3] == nullptr &&
 _lauta[r][0] == (vari ==0 ? vt : mt)) {
 Ruutu r3(r,3), r2(r,2);
 if (!onkoRuutuUhattu(&r3,1 - vari) && !onkoRuutuUhattu(&r2,1 - vari)) {
 lista.push_back(Siirto(false, true));
 }
 }
 }
}

void Asema::annaLaillisetSiirrot(std::list<Siirto>& lista)
{
 lista.clear();

 //1) raakasiirrot
 for (int r =0; r <8; r++) {
 for (int s =0; s <8; s++) {
 Nappula* n = _lauta[r][s];
 if (n != nullptr && n->getVari() == _siirtovuoro) {
 Ruutu ruutu(r, s);
 n->annaSiirrot(lista, &ruutu, this, _siirtovuoro);
 }
 }
 }

 //2) linnoitus
 annaLinnoitusSiirrot(lista, _siirtovuoro);

 //3) kuninkaan ruutu
 Ruutu kuninkaanRuutu;
 for (int r =0; r <8; r++) {
 for (int s =0; s <8; s++) {
 if (_lauta[r][s] != nullptr) {
 if ((_siirtovuoro ==0 && _lauta[r][s] == vk) ||
 (_siirtovuoro ==1 && _lauta[r][s] == mk)) {
 kuninkaanRuutu = Ruutu(r, s);
 }
 }
 }
 }

 int vastustaja =1 - _siirtovuoro;

 //4) suodatus: poistetaan siirrot, jotka jättävät kuningaan shakkiin
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

 if (siirtyva && siirtyva->getKoodi() == VS && _siirtovuoro ==0) {
 if (ar ==4 && lr ==5 && as != ls && syoty == nullptr && ls == kaksoisaskelSarakkeella) {
 enPassant = true;
 eatenR =4;
 eatenS = ls;
 }
 }
 if (siirtyva && siirtyva->getKoodi() == MS && _siirtovuoro ==1) {
 if (ar ==3 && lr ==2 && as != ls && syoty == nullptr && ls == kaksoisaskelSarakkeella) {
 enPassant = true;
 eatenR =3;
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

 //1) SOTILAAT (vari = hyökkääjän väri)
 // valkea sotilas lyö (r-1, s±1), musta (r+1, s±1), koska r kasvaa alhaalta ylöspäin
 int sr = (vari ==0 ? r -1 : r +1);
 if (sr >=0 && sr <8) {
 if (s -1 >=0 && _lauta[sr][s -1] == (vari ==0 ? vs : ms)) return true;
 if (s +1 <8 && _lauta[sr][s +1] == (vari ==0 ? vs : ms)) return true;
 }

 //2) RATSUT
 int drN[8] = {2,2,-2,-2,1,1,-1,-1 };
 int dsN[8] = {1,-1,1,-1,2,-2,2,-2 };
 for (int i =0; i <8; i++) {
 int nr = r + drN[i], ns = s + dsN[i];
 if (nr >=0 && nr <8 && ns >=0 && ns <8) {
 if (_lauta[nr][ns] == (vari ==0 ? vr : mr)) return true;
 }
 }

 //3) TORNI + DAAMI (suorat linjat)
 int dirs[4][2] = { {1,0},{-1,0},{0,1},{0,-1} };
 for (auto& d : dirs) {
 int nr = r + d[0], ns = s + d[1];
 while (nr >=0 && nr <8 && ns >=0 && ns <8) {
 if (_lauta[nr][ns] != nullptr) {
 if (_lauta[nr][ns] == (vari ==0 ? vt : mt) ||
 _lauta[nr][ns] == (vari ==0 ? vd : md))
 return true;
 break;
 }
 nr += d[0]; ns += d[1];
 }
 }

 //4) LÄHETTI + DAAMI (diagonaalit)
 int d2[4][2] = { {1,1},{1,-1},{-1,1},{-1,-1} };
 for (auto& d : d2) {
 int nr = r + d[0], ns = s + d[1];
 while (nr >=0 && nr <8 && ns >=0 && ns <8) {
 if (_lauta[nr][ns] != nullptr) {
 if (_lauta[nr][ns] == (vari ==0 ? vl : ml) ||
 _lauta[nr][ns] == (vari ==0 ? vd : md))
 return true;
 break;
 }
 nr += d[0]; ns += d[1];
 }
 }

 //5) KUNINGAS (viereiset ruudut)
 for (int drr = -1; drr <=1; drr++) {
 for (int dss = -1; dss <=1; dss++) {
 if (drr ==0 && dss ==0) continue;
 int nr = r + drr, ns = s + dss;
 if (nr >=0 && nr <8 && ns >=0 && ns <8) {
 if (_lauta[nr][ns] == (vari ==0 ? vk : mk)) return true;
 }
 }
 }

 return false;
}


void Asema::huolehdiKuninkaanShakeista(std::list<Siirto>&, int) {}
