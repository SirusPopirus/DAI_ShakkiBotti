#include <Windows.h>
#include <iostream>
#include <string>
#include <fcntl.h>
#include <io.h>
#include "kayttoliittyma.h"
#include "nappula.h"
#include <algorithm>
#include <cctype>
#include <iomanip>

using namespace std;

Kayttoliittyma* Kayttoliittyma::instance =0;

Kayttoliittyma* Kayttoliittyma::getInstance()
{
 if (instance ==0)
 instance = new Kayttoliittyma();
 return instance;
}

static string trim(const string& s) {
 size_t start = s.find_first_not_of(" \t\r\n");
 if (start == string::npos) return "";
 size_t end = s.find_last_not_of(" \t\r\n");
 return s.substr(start, end - start +1);
}

static bool parseSquare(const string& sq, Ruutu& ruutu)
{
 if (sq.size() !=2) return false;
 char file = tolower(sq[0]);
 char rank = sq[1];
 if (file < 'a' || file > 'h') return false;
 if (rank < '1' || rank > '8') return false;
 int sarake = file - 'a';
 int rivi = rank - '1';
 ruutu = Ruutu(rivi, sarake);
 return true;
}

void Kayttoliittyma::piirraLauta()
{
 if (this->_asema == nullptr) return;

 HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
 _setmode(_fileno(stdout), _O_U16TEXT);

 const WORD BG_VALKEA = BACKGROUND_GREEN | BACKGROUND_INTENSITY;
 const WORD BG_TUMMA = BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_INTENSITY;

 const WORD FG_VALKOINEN = FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
 const WORD FG_MUSTA =0;

 const int cellWidth =3;
 const int rankLabelWidth =2; // e.g. "8 " printed before cells

 // Build header so letters are centered above cells
 std::wstring header;
 header.append(rankLabelWidth, L' ');
 for (int j =0; j <8; ++j) {
 std::wstring cell(cellWidth, L' ');
 int pos = cellWidth /2;
 cell[pos] = L'a' + j;
 header += cell;
 }

 wcout << L"\n" << header << endl;

 for (int i =7; i >=0; --i) {
 SetConsoleTextAttribute(hConsole, FG_VALKOINEN);
 // rank label + space
 wcout << i +1 << L" ";

 for (int j =0; j <8; ++j) {
 bool squareIsDark = ((i + j) %2) !=0;
 WORD bg = squareIsDark ? BG_TUMMA : BG_VALKEA;

 Nappula* n = _asema->_lauta[i][j];

 if (n == nullptr) {
 SetConsoleTextAttribute(hConsole, bg);
 for (int k =0; k < cellWidth; ++k) wcout << L' ';
 }
 else {
 int vari = n->getVari();
 WORD fg = (vari ==0) ? FG_VALKOINEN : FG_MUSTA;

 SetConsoleTextAttribute(hConsole, bg | fg);

 std::wstring piece = n->getUnicode();

 int glyphWidth = (int)piece.length();
 int padLeft = (cellWidth - glyphWidth) /2;
 int padRight = cellWidth - glyphWidth - padLeft;

 for (int k =0; k < padLeft; ++k) wcout << L' ';
 wcout << piece;
 for (int k =0; k < padRight; ++k) wcout << L' ';
 }
 }

 SetConsoleTextAttribute(hConsole, FG_VALKOINEN);
 wcout << L" " << i +1 << endl;
 }

 // Footer (same as header)
 wcout << L"\n" << header << L"\n" << endl;
 _setmode(_fileno(stdout), _O_TEXT);

 // Show evaluation and side to move in normal (narrow) output
 double eval =0.0;
 try { eval = _asema->evaluoi(); } catch (...) { eval =0.0; }

 cout << fixed << setprecision(2);
 cout << "Evaluointi (valkea +): " << eval << "\n";
 int vuoro = _asema->getSiirtovuoro();
 cout << "Siirtovuoro: " << (vuoro ==0 ? "valkea" : "musta") << "\n\n";
}

Siirto Kayttoliittyma::annaVastustajanSiirto()
{
 while (true) {
 string line;
 cout << "Anna vastustajan siirto (esim Rg1-f3): ";
 getline(cin, line);

 line = trim(line);
 if (line.empty()) {
 cout << "tyhjä syöte, koita uusiks\n";
 continue;
 }

 string upper = line;
 transform(upper.begin(), upper.end(), upper.begin(),
 [](unsigned char c) { return static_cast<char>(toupper(c)); });
 if (upper == "O-O") {
 return Siirto(true, false);
 }
 if (upper == "O-O-O") {
 return Siirto(false, true);
 }

 size_t dash = line.find("-");
 if (dash == string::npos) {
 cout << "Virhe: ei ole viivaa -\n";
 continue;
 }
 string left = trim(line.substr(0, dash));
 string right = trim(line.substr(dash +1));

 Ruutu alku;
 Ruutu loppu;
 bool ok = false;

 if (left.size() ==2) {
 if (parseSquare(left, alku) && parseSquare(right, loppu)) {
 return Siirto(alku, loppu);
 }
 else {
 cout << "virhesyöttö, yritä uusiks\n";
 continue;
 }
 }

 if (left.size() ==3) {
 char piece = static_cast<char>(toupper(static_cast<unsigned char>(left[0])));
 if (piece == 'T' || piece == 'R' || piece == 'L' || piece == 'D' || piece == 'K') {
 string sq = left.substr(1);
 if (parseSquare(sq, alku) && parseSquare(right, loppu)) {
 ok = true;
 }
 }
 else {
 cout << "outo nappula " << left[0] << " (sallitut: T, R, L, D, K)\n";
 }
 }
 else {
 cout << "virhe: pitää olla kirjain + ruutu, esim Rg1\n";
 }
 if (!ok) {
 cout << "siirto ei toiminut, yritä uusiks\n";
 continue;
 }
 return Siirto(alku, loppu);
 }
}

int Kayttoliittyma::kysyVastustajanVari()
{
 while (true) {
 cout << "valkea (v) vai musta (m)? ";
 string line;
 getline(cin, line);
 line = trim(line);
 if (line.empty()) {
 cout << "tyhjä syöte, yritä uusiks\n";
 continue;
 }
 char c = static_cast<char>(tolower(static_cast<unsigned char>(line[0])));
 if (c == 'v') return 0;
 if (c == 'm') return 1;
 cout << "Anna 'v' tai 'm'\n";
 }
}
