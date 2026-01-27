#include "siirto.h"


Siirto::Siirto(Ruutu _alkuRuutu, Ruutu _loppuRuutu)
{
	this->_alkuRuutu = _alkuRuutu;
	this->_loppuRuutu = _loppuRuutu;
	this->_lyhytLinna = false;
	this->_pitkaLinna = false;
}


Siirto::Siirto(bool _lyhytLinna, bool _pitkaLinna) 
{
	this->_lyhytLinna = _lyhytLinna;
	this->_pitkaLinna = _pitkaLinna;
	
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
