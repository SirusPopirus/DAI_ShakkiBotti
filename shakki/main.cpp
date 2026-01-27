#include <iostream>
#include <Windows.h>
#include <io.h>
#include <fcntl.h>
#include <iostream>
#include <string>
#include "kayttoliittyma.h"
#include "Siirto.h"
#include "asema.h"

using namespace std; 

int main()
{
	// 1. Luodaan asema-olio (tämä varaa muistin ja alustaa nappulat)
	Asema asema;

	// 2. Haetaan käyttöliittymä ja kerrotaan sille, missä tuo 'asema' sijaitsee
	Kayttoliittyma::getInstance()->aseta_asema(&asema);

	// 3. Nyt piirtäminen onnistuu, koska _asema-osoitin on asetettu

	while (true)
	{
		Kayttoliittyma::getInstance()->piirraLauta();

		// Estetään ikkunaa sulkeutumasta heti
		// system("pause");
		wcout << L"(kirjoita exit lopettaaksesi)\n";
		wstring syote;
		getline(wcin, syote);

		if (syote == L"exit") break;

		Siirto s = Kayttoliittyma::getInstance()->annaVastustajanSiirto();

		asema.paivitaAsema(&s);
	}

	system("pause");


	return 0;
}


	//wcout << "HeippariShakki\n";
	//wcout << "Tervetuloa pelaamaan!\n";
	//int lopetus = 100;
	//Asema asema; 
	//Kayttoliittyma::getInstance()->aseta_asema(&asema);

	//Peli peli(Kayttoliittyma::getInstance()->
	//	kysyVastustajanVari());
	//std::list<Siirto> lista;
	//system("cls");
	//int koneenVari = peli.getKoneenVari();

	//while (lopetus != 0) {
	//	lista.clear();
	//	Kayttoliittyma::getInstance()->piirraLauta();
	//	wcout << "\n";
	//	// Tarkasta onko peli loppu?
	//	asema.annaLaillisetSiirrot(lista);
	//	if (lista.size() == 0) {
	//		lopetus = 0;
	//		std::wcout << "Peli loppui";
	//		continue;
	//	}
	//	Siirto siirto;
	//	if (asema.getSiirtovuoro() == koneenVari) {
	//		MinMaxPaluu paluu;
	//		if (koneenVari == 0) {
	//			paluu = asema.maxi(3);
	//		}
	//		else {
	//			paluu = asema.mini(3);
	//		}
	//		siirto = paluu._parasSiirto;
	//	}
	//	else {
	//		siirto = Kayttoliittyma::getInstance()->
	//			annaVastustajanSiirto();
	//	}
	//	asema.paivitaAsema(&siirto);
	//}

