#include <iostream>
#include <list>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include "kayttoliittyma.h"
#include "asema.h"
#include "siirto.h"

using namespace std;

bool siirrotSamat(Siirto& a, Siirto& b)
{
    // linnoitus
    if (a.onkoLyhytLinna() && b.onkoLyhytLinna()) return true;
    if (a.onkoPitkalinna() && b.onkoPitkalinna()) return true;

    // normaali siirto
    return
        !a.onkoLyhytLinna() && !a.onkoPitkalinna() &&
        !b.onkoLyhytLinna() && !b.onkoPitkalinna() &&
        a.getAlkuruutu().getRivi() == b.getAlkuruutu().getRivi() &&
        a.getAlkuruutu().getSarake() == b.getAlkuruutu().getSarake() &&
        a.getLoppuruutu().getRivi() == b.getLoppuruutu().getRivi() &&
        a.getLoppuruutu().getSarake() == b.getLoppuruutu().getSarake();
}

int main()
{
    Asema asema;
    Kayttoliittyma* ui = Kayttoliittyma::getInstance();
    ui->aseta_asema(&asema);

    while (true) {
        system("cls");
        ui->piirraLauta();

        //1) Generoidaan lailliset siirrot
        list<Siirto> lailliset;
        asema.annaLaillisetSiirrot(lailliset);

        cout << "Syötä siirto (esim e2-e4 tai Rg1-f3). Tyhjä syöte lopettaa.\n";

        Siirto syote;
        bool ok = false;

        while (!ok) {
            syote = ui->annaVastustajanSiirto();

            //2) Tarkistetaan löytyykö siirto listasta
            for (auto& s : lailliset) {
                if (siirrotSamat(s, syote)) {
                    // Use the legal move (includes promotion choice) instead of plain input
                    syote = s;
                    ok = true;
                    break;
                }
            }

            if (!ok) {
                cout << "Laiton siirto! Yritä uudestaan.\n";
            }
        }

        //3) Suoritetaan siirto
        asema.paivitaAsema(&syote);
    }

    return 0;
}
