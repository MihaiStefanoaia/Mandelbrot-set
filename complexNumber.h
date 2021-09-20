#pragma once
#include <iostream>
#include <string>

class complexNumber {
public:
	double realPart;
	double imaginaryPart;

public:

	complexNumber(double real=0, double imaginary=0);
	complexNumber(const complexNumber& a);

	std::string printable();
	friend std::ostream& operator<<(std::ostream& os, complexNumber const& c);

	complexNumber& operator=(const complexNumber& a);

	complexNumber& operator+=(complexNumber a);
	complexNumber& operator-=(complexNumber a);
	complexNumber& operator*=(complexNumber a);
	complexNumber& operator/=(complexNumber a);

	friend complexNumber operator+(complexNumber a, const complexNumber& b)
	{
		complexNumber tmp(a);
		return tmp += b;
	};
	friend complexNumber operator-(complexNumber a, const complexNumber& b)
	{
		complexNumber tmp(a);
		return tmp -= b;
	};
	friend complexNumber operator*(complexNumber a, const complexNumber& b)
	{
		complexNumber tmp(a);
		return tmp *= b;
	};
	friend complexNumber operator/(complexNumber a, const complexNumber& b)
	{
		complexNumber tmp(a);
		return tmp /= b;
	};
};

