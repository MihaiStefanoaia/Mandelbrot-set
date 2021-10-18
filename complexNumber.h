#pragma once
#include <iostream>
#include <string>

class complexNumber {
public:
	float realPart;
	float imaginaryPart;

public:

	complexNumber(float real=0, float imaginary=0);
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

