#include "complexNumber.h"

complexNumber::complexNumber(float real, float imaginary)
{
	this->realPart = real;
	this->imaginaryPart = imaginary;
}

complexNumber::complexNumber(const complexNumber& a)
{
	this->realPart = a.realPart;
	this->imaginaryPart = a.imaginaryPart;
}

std::string complexNumber::printable()
{
	std::string tmp;
	tmp = std::to_string(this->realPart) + " + " + std::to_string(this->imaginaryPart) + "i";

	return tmp;
}

std::ostream& operator<<(std::ostream& os, complexNumber const& nr)
{
	os << std::string(std::to_string(nr.realPart) + " + " + std::to_string(nr.imaginaryPart) + "i");
	return os;
}

complexNumber& complexNumber::operator=(const complexNumber& a)
{
	this->realPart = a.realPart;
	this->imaginaryPart = a.imaginaryPart;
	return *this;
}

complexNumber& complexNumber::operator+=(complexNumber a)
{
	this->realPart += a.realPart;
	this->imaginaryPart += a.imaginaryPart;
	return *this;
}

complexNumber& complexNumber::operator-=(complexNumber a)
{
	this->realPart -= a.realPart;
	this->imaginaryPart -= a.imaginaryPart;
	return *this;
}

complexNumber& complexNumber::operator*=(complexNumber a) // (ac-bd) + (ab+cd)i
{
	complexNumber tmp(0,0);
	tmp.realPart = this->realPart * a.realPart - this->imaginaryPart * a.imaginaryPart; //ac - bd
	tmp.imaginaryPart = a.imaginaryPart * this->realPart + a.realPart * this->imaginaryPart; //(ab+cd)i

	this->realPart = tmp.realPart;
	this->imaginaryPart = tmp.imaginaryPart;

	return *this;
}

complexNumber& complexNumber::operator/=(complexNumber a) // (ac + bd)/(c^2 + d^2) + (bc - ad)/(c^2 + d^2)i
{
	complexNumber tmp(0,0);
	tmp.realPart = (this->realPart * a.realPart + this->imaginaryPart * a.imaginaryPart) / (a.realPart * a.realPart + a.imaginaryPart * a.imaginaryPart);
	tmp.imaginaryPart = (this->imaginaryPart * a.realPart - this->realPart * a.imaginaryPart) / (a.realPart * a.realPart + a.imaginaryPart * a.imaginaryPart);

	this->realPart = tmp.realPart;
	this->imaginaryPart = tmp.imaginaryPart;

	return *this;
}