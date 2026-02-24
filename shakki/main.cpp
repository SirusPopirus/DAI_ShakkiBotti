#include <iostream>
#include <list>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <thread>
#include <chrono>
#include "kayttoliittyma.h"
#include "asema.h"
#include "siirto.h"
#include "minmaxpaluu.h"

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

static void tulostaSiirto(Siirto& m)
{
    if (m.onkoLyhytLinna()) { cout << "O-O"; return; }
    if (m.onkoPitkalinna()) { cout << "O-O-O"; return; }
    Ruutu a = m.getAlkuruutu();
    Ruutu b = m.getLoppuruutu();
    char fa = 'a' + a.getSarake();
    char ra = '1' + a.getRivi();
    char fb = 'a' + b.getSarake();
    char rb = '1' + b.getRivi();
    cout << fa << ra << '-' << fb << rb;
}

int main()
{
    Asema asema;
    Kayttoliittyma* ui = Kayttoliittyma::getInstance();
    ui->aseta_asema(&asema);

    const int botDepth = 3; // search depth for black bot

    while (true) {
        system("cls");
        ui->piirraLauta();

        // if it's black's turn, let the bot play automatically
        if (asema.getSiirtovuoro() == 1) {
            cout << "Bot (musta) laskee siirtoa...\n";
            // ensure there are legal moves
            list<Siirto> laillisetBot;
            asema.annaLaillisetSiirrot(laillisetBot);
            if (laillisetBot.empty()) {
                cout << "Botilla ei laillisia siirtoja. Peli ohi.\n";
                break;
            }

            MinMaxPaluu mm = asema.maxi(botDepth);
            Siirto botSiirto = mm._parasSiirto;

            cout << "Bot siirtää: "; tulostaSiirto(botSiirto); cout << "\n";
            // small pause so user can see the move
            this_thread::sleep_for(chrono::milliseconds(700));

            asema.paivitaAsema(&botSiirto);
            continue; // redraw board after bot move
        }

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
