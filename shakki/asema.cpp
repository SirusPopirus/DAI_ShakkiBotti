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
	// 1. Alustetaan kaikki tyhjäksi (nullptr)
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			_lauta[i][j] = nullptr;

	// 2. Valkoiset upseerit (rivi 0) ja sotilaat (rivi 1)
	_lauta[0][0] = vt; _lauta[0][1] = vr; _lauta[0][2] = vl; _lauta[0][3] = vd;
	_lauta[0][4] = vk; _lauta[0][5] = vl; _lauta[0][6] = vr; _lauta[0][7] = vt;
	for (int i = 0; i < 8; i++) _lauta[1][i] = vs;

	// 3. Mustat upseerit (rivi 7) ja sotilaat (rivi 6)
	_lauta[7][0] = mt; _lauta[7][1] = mr; _lauta[7][2] = ml; _lauta[7][3] = md;
	_lauta[7][4] = mk; _lauta[7][5] = ml; _lauta[7][6] = mr; _lauta[7][7] = mt;
	for (int i = 0; i < 8; i++) _lauta[6][i] = ms;
	// Ensin alustetaan kaikki laudan ruudut nappulla "NULL", koska muuten ruuduissa satunnaista tauhkaa

	// Asetetaan alkuaseman mukaisesti nappulat ruuduille
	
}



void Asema::paivitaAsema(Siirto *siirto)
{

	// Kaksoisaskel-lippu on oletusarvoisesti pois pддltд.
	// Asetetaan myцhemmin, jos tarvii.


	//Tarkastetaan on siirto lyhyt linna


	// onko pitkд linna



	// Kaikki muut siirrot


		//Ottaa siirron alkuruudussa olleen nappulan talteen 


		//Laittaa talteen otetun nappulan uuteen ruutuun


		// Tarkistetaan oliko sotilaan kaksoisaskel
		// (asetetaan kaksoisaskel-lippu)

		// Ohestalyцnti on tyhjддn ruutuun. Vieressд oleva (sotilas) poistetaan.

		//// Katsotaan jos nappula on sotilas ja rivi on pддtyrivi niin ei vaihdeta nappulaa 
		////eli alkuruutuun laitetaan null ja loppuruudussa on jo kliittymдn laittama nappula MIIKKA, ei taida minmaxin kanssa hehkua?

		//
		////muissa tapauksissa alkuruutuun null ja loppuruutuun sama alkuruudusta lдhtenyt nappula

		// katsotaan jos liikkunut nappula on kuningas niin muutetaan onkoKuningasLiikkunut arvo (molemmille vдreille)

		// katsotaan jos liikkunut nappula on torni niin muutetaan onkoTorniLiikkunut arvo (molemmille vдreille ja molemmille torneille)

	//pдivitetддn _siirtovuoro

}



int Asema::getSiirtovuoro() 
{
	return 0;
}


void Asema::setSiirtovuoro(int vuoro) 
{
	
}


bool Asema::getOnkoValkeaKuningasLiikkunut() 
{
	return false;
}


bool Asema::getOnkoMustaKuningasLiikkunut() 
{
	return false;
}


bool Asema::getOnkoValkeaDTliikkunut() 
{
	return false;
}


bool Asema::getOnkoValkeaKTliikkunut() 
{
	return false;
}


bool Asema::getOnkoMustaDTliikkunut() 
{
	return false;
}


bool Asema::getOnkoMustaKTliikkunut() 
{
	return false;
}


/* 1. Laske nappuloiden arvo
Daami = 9
Torni = 5
Lдhetti = 3,25
Ratsu = 3
Sotilas = 1

2. Kuninkaan hyvyys
Jos avaus tai keskipeli, niin hyvд ettд kunigas g1 tai b1/c1
Loppupelissд vaikea sanoa halutaanko olla auttamassa omaa sotilasta korottumaan
vai olla estдmдssд vastustajan korotusta siksi ei oteta kantaa
3. Arvosta keskustaa sotilailla ja ratsuilla
4. Arvosta pitkiд linjoja daami, torni ja lдhetti
*/
double Asema::evaluoi() 
{
	return 0;

	//kertoimet asetettu sen takia ettд niiden avulla asioiden painoarvoa voidaan sддtдд helposti yhdestд paikasta
	
	//1. Nappuloiden arvo
	
	//2. Kuningas turvassa
	
	//3. Arvosta keskustaa
	
	// 4. Arvosta linjoja
	
}


double Asema::laskeNappuloidenArvo(int vari) 
{
	return 0;
	
}


bool Asema::onkoAvausTaiKeskipeli(int vari) 
{
	return 0;
	// Jos upseereita 3 tai vдhemmдn on loppupeli
	// mutta jos daami laudalla on loppueli vasta kun kuin vain daami jдljellд
	
	//Jos vari on 0 eli valkoiset
	//niin on keskipeli jos mustalla upseereita yli 2 tai jos daami+1
	

}


double Asema::nappuloitaKeskella(int vari) 
{
	return 0;

	//sotilaat ydinkeskustassa + 0.25/napa
	//ratsut ydinkeskustassa + 0.25/napa
	//sotilaat laitakeskustassa + 0.11/napa
	//ratsut laitakeskustassa + 0.11/napa
	
	//valkeille ydinkeskusta

	
	
	//valkeille laitakeskusta
	


	//mustille ydinkeskusta
	
	//mustille laitakeskusta
	
}


double Asema::linjat(int vari) 
{
	return 0;
	
	//valkoiset
	
	//mustat
	
}


// https://chessprogramming.wikispaces.com/Minimax MinMax-algoritmin pseudokoodi (lisдsin parametrina aseman)
//int maxi(int depth, asema a) 
//	if (depth == 0) return evaluate();
//	int max = -oo;
//	for (all moves ) {
//		score = mini(depth - 1, seuraaja);
//		if (score > max)
//			max = score;
//	}
//	return max;
//}

//int mini(int depth, asema a) {
//	if (depth == 0) return -evaluate();
//	int min = +oo;
//	for (all moves) {
//		score = maxi(depth - 1);
//		if (score < min)
//			min = score;
//	}
//	return min;
//}
MinMaxPaluu Asema::minimax(int syvyys)
{
	MinMaxPaluu paluuarvo;

	// Generoidaan aseman lailliset siirrot.
	
	// Rekursion kantatapaus 1: peli on loppu
	
	// Rekursion kantatapaus 2: katkaisusyvyydessд
	
	// Rekursioaskel: kokeillaan jokaista laillista siirtoa s
	// (alustetaan paluuarvo huonoimmaksi mahdolliseksi).
	
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


void Asema::annaLaillisetSiirrot(std::list<Siirto>& lista) {
	
}
