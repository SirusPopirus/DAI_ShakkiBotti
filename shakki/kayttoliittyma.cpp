#include <Windows.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>
#include <iostream>
#include "kayttoliittyma.h"


using namespace std;


Kayttoliittyma* Kayttoliittyma::instance = 0;

Kayttoliittyma* Kayttoliittyma::getInstance()
{
	if (instance == 0)
		instance = new Kayttoliittyma();
	return instance;
}

void Kayttoliittyma::piirraLauta()
{
    if (this->_asema == nullptr) return;

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    // Unicode-tila
    _setmode(_fileno(stdout), _O_U16TEXT);

    // Ruutujen värit
    WORD valkeaRuutu =
        BACKGROUND_GREEN | BACKGROUND_INTENSITY |
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    WORD tummaRuutu =
        BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY |
        FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;

    wcout << L"\n     a  b  c  d  e  f  g  h" << endl;

    for (int i = 7; i >= 0; i--) {

        // Rivin numero
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        wcout << i + 1 << L"  ";

        for (int j = 0; j < 8; j++) {

            // Ruudun väri
            SetConsoleTextAttribute(
                hConsole,
                ((i + j) % 2) ? tummaRuutu : valkeaRuutu
            );

            Nappula* n = _asema->_lauta[i][j];

            if (n == nullptr) {
                wcout << L"   ";
            }
            else {
                wcout << L" " << n->getUnicode() << L" ";
            }
        }

        // Normaali väri rivin loppuun
        SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
        wcout << L"  " << i + 1 << endl;
    }

    wcout << L"     a  b  c  d  e  f  g  h\n" << endl;

    _setmode(_fileno(stdout), _O_TEXT);
}



/*
	Aliohjelma tarkistaa ettд kдyttдjдn antama syцte siirroksi on 
	muodollisesti korrekti (ei tarkista aseman laillisuutta)
	Ottaa irti myцs nappulan kirjaimen (K/D/L/R/T), tarkistaa ettд kirjain korrekti
*/
Siirto Kayttoliittyma::annaVastustajanSiirto()
{
    wstring syote;
    wcout << L"Anna siirto: ";
    getline(wcin, syote);

    if (syote == L"O-O")
        return Siirto(true, false);

    if (syote == L"O-O-O")
        return Siirto(false, true);

    int alkuSarake = syote[1] - L'a';
    int alkuRivi = syote[2] - L'1';

    int loppuSarake = syote[4] - L'a';
    int loppuRivi = syote[5] - L'1';

    return Siirto(Ruutu(alkuRivi, alkuSarake),
        Ruutu(loppuRivi, loppuSarake));
}


int Kayttoliittyma::kysyVastustajanVari()
{
	return 0;
}
