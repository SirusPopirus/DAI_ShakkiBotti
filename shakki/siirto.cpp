#include "siirto.h"

Siirto::Siirto(Ruutu alku, Ruutu loppu)
{
    _alkuRuutu = alku;
    _loppuRuutu = loppu;
    _lyhytLinna = false;
    _pitkaLinna = false;
}

Siirto::Siirto(bool lyhytLinna, bool pitkaLinna)
{
    _lyhytLinna = lyhytLinna;
    _pitkaLinna = pitkaLinna;
    _alkuRuutu = Ruutu();
    _loppuRuutu = Ruutu();
}

Ruutu Siirto::getAlkuruutu()
{
    return _alkuRuutu;
}

Ruutu Siirto::getLoppuruutu()
{
    return _loppuRuutu;
}

bool Siirto::onkoLyhytLinna()
{
    return _lyhytLinna;
}

bool Siirto::onkoPitkälinna()
{
    return _pitkaLinna;
}
