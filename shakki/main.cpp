#include <iostream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <string>
#include "kayttoliittyma.h"
#include "siirto.h"
#include "asema.h"

using namespace std;

int main()
{
    Asema asema;
    Kayttoliittyma* ui = Kayttoliittyma::getInstance();
    ui->aseta_asema(&asema);

    while (true) {
        system("cls");
        ui->piirraLauta();

        cout << "syötä siirto esim Rg1-f3: tyhjä syöttä lopettaa\n";
        Siirto s = ui->annaVastustajanSiirto();
        asema.paivitaAsema(&s);
    }

    return 0;
}
