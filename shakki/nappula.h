#pragma once

#include <list>
#include <string>
#include "asema.h"
#include "siirto.h"

enum
{
    VT, VR, VL, VD, VK, VS,
    MT, MR, ML, MD, MK, MS
};

class Nappula
{
private:
    std::wstring _unicode;
    int _vari;   // valkea = 0, musta = 1
    int _koodi;  // VT, VR, MT tms.

public:
    Nappula(std::wstring, int, int);
    Nappula() {}

    virtual void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) = 0;

    void setUnicode(std::wstring unicode) { _unicode = unicode; }
    std::wstring getUnicode() { return _unicode; }
    void setVari(int vari) { _vari = vari; }
    int getVari() { return _vari; }
    int getKoodi() { return _koodi; }
    void setKoodi(int koodi) { _koodi = koodi; }
};

class Torni : public virtual Nappula {
public:
    Torni(std::wstring unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
    void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

class Ratsu : public Nappula {
public:
    Ratsu(std::wstring unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
    void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

class Lahetti : public virtual Nappula {
public:
    Lahetti(std::wstring unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
    void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

class Daami : public Lahetti, public Torni {
public:
    Daami(std::wstring unicode, int vari, int koodi) :
        Nappula(unicode, vari, koodi), Lahetti(unicode, vari, koodi), Torni(unicode, vari, koodi) {
    }
    void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

class Kuningas : public Nappula {
public:
    Kuningas(std::wstring unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
    void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
};

class Sotilas : public Nappula {
public:
    Sotilas(std::wstring unicode, int vari, int koodi) : Nappula(unicode, vari, koodi) {}
    void annaSiirrot(std::list<Siirto>& lista, Ruutu*, Asema*, int vari) override;
private:
    void lisaaSotilaanKorotukset(Siirto*, std::list<Siirto>& lista, Asema*);
};
