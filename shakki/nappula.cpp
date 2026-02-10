#include <list>
#include <string>
#include "asema.h"
#include "nappula.h"
#include "siirto.h"
using namespace std;


Nappula::Nappula(wstring unicode, int vari, int koodi)
{
	_unicode = unicode;
	_vari = vari;
	_koodi = koodi;

}


void Torni::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int r = ruutu->getRivi();
	int s = ruutu->getSarake();

	int dr[4] = { 1, -1, 0, 0 };
	int ds[4] = { 0, 0, 1, -1 };

	for (int dir = 0; dir < 4; dir++) {
		int rr = r + dr[dir];
		int ss = s + ds[dir];
		while (rr >= 0 && rr < 8 && ss >= 0 && ss < 8) {
			Nappula* n = asema->_lauta[rr][ss];
			if (n == nullptr) {
				lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ss)));
			}
			else {
				if (n->getVari() != vari) {
					lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ss)));
				}
				break;
			}
			rr += dr[dir];
			ss += ds[dir];
		}
	}

}


void Ratsu::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int r = ruutu->getRivi();
	int s = ruutu->getSarake();

	int dr[8] = { 2, 2, 1, 1, -1, -1, -2, -2 };
	int ds[8] = { 1, -1, 2, -2, 2, -2, 1, -1 };
	for (int i = 0; i < 8; i++) {
		int rr = r + dr[i];
		int ss = s + ds[i];

		if (rr < 0 || rr > 7 || ss < 0 || ss > 7)
			continue;

		Nappula* n = asema->_lauta[rr][ss];

		if (n == nullptr) {
			lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ss)));
		}
		else if (n->getVari() != vari) {
			lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ss)));
		}
	}

}


void Lahetti::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int r = ruutu->getRivi();
	int s = ruutu->getSarake();

	int dr[4] = { 1, 1, -1, -1 };
	int ds[4] = { 1, -1, 1, -1 };

	for (int dir = 0; dir < 4; dir++) {
		int rr = r + dr[dir];
		int ss = s + ds[dir];

		while (rr >= 0 && rr < 8 && ss >= 0 && ss < 8) {
			Nappula* n = asema->_lauta[rr][ss];
			if (n == nullptr) {
				lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ss)));
			}
			else {
				if (n->getVari() != vari) {
					lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ss)));
				}
				break;
			}
			rr += dr[dir];
			ss += ds[dir];
		}
	}

}


void Daami::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	Torni torni(this->getUnicode(), vari, this->getKoodi());
	Lahetti lahetti(this->getUnicode(), vari, this->getKoodi());

	torni.annaSiirrot(lista, ruutu, asema, vari);
	lahetti.annaSiirrot(lista, ruutu, asema, vari);

}


void Kuningas::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int r = ruutu->getRivi();
	int s = ruutu->getSarake();

	int dr[8] = { 1, 1, 1, 0, 0, -1, -1, -1 };
	int ds[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };

	for (int i = 0; i < 8; i++) {
		int rr = r + dr[i];
		int ss = s + ds[i];

		if (rr < 0 || rr > 7 || ss < 0 || ss > 7)
			continue;

		Nappula* n = asema->_lauta[rr][ss];

		if (n == nullptr) {
			lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ss)));
		}
		else if (n->getVari() != vari) {
			lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ss)));
		}
	}

}


void Sotilas::annaSiirrot(std::list<Siirto>& lista, Ruutu* ruutu, Asema* asema, int vari)
{
	int r = ruutu->getRivi();
	int s = ruutu->getSarake();
	int suunta = (vari == 0 ? 1 : -1);

	int rr = r + suunta;
	int ss = s;

	if (rr >= 0 && rr < 8 && asema->_lauta[rr][ss] == nullptr) {
		if (rr == 7 || rr == 0) {
			Siirto siirto(Ruutu(r, s), Ruutu(rr, ss));
			lisaaSotilaanKorotukset(&siirto, lista, asema);
		}
		else {
			lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ss)));
		}

		int alotusrivi = (vari == 0 ? 1 : 6);
		if (r == alotusrivi) {
			int rr2 = r + 2 * suunta;
			if (asema->_lauta[rr2][ss] == nullptr) {
				lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr2, ss)));
			}
		}
	}
	int diagS[2] = { s - 1, s + 1 };

	for (int i = 0; i < 2; i++) {
		int ds = diagS[i];
		if (rr >= 0 && rr < 8 && ds >= 0 && ds < 8) {
			Nappula* n = asema->_lauta[rr][ds];
			if (n != nullptr && n->getVari() != vari) {
				if (rr == 7 || rr == 0) {
					Siirto siirto(Ruutu(r, s), Ruutu(rr, ds));
					lisaaSotilaanKorotukset(&siirto, lista, asema);
				}
				else {
					lista.push_back(Siirto(Ruutu(r, s), Ruutu(rr, ds)));
				}
			}
		}
	}

}


void Sotilas::lisaaSotilaanKorotukset(Siirto* siirto, std::list<Siirto>& lista, Asema* asema)
{
	int alkuRivi = siirto->getAlkuruutu().getRivi();
	bool valkea = (alkuRivi == 6);

	Siirto d = *siirto;
	d._miksikorotetaan = (valkea ? Asema::vd : Asema::md);
	lista.push_back(d);

	Siirto t = *siirto;
	t._miksikorotetaan = (valkea ? Asema::vt : Asema::mt);
	lista.push_back(t);

	Siirto l = *siirto;
	l._miksikorotetaan = (valkea ? Asema::vl : Asema::ml);
	lista.push_back(l);

	Siirto r = *siirto;
	r._miksikorotetaan = (valkea ? Asema::vr : Asema::mr);
	lista.push_back(r);

}