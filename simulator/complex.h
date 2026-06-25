#pragma once

#include <iostream>
#include <string>

class Complex{
	public:
		double r;  // real component
		double i;  // imaginary component

		// constructor
		Complex(double, double);

		// complex conjugate, module and power
		Complex cc();
		double m();
		double m2();
		Complex cpow(int);

		// string features
		std::string str(int) const;
		friend std::string operator+(Complex, std::string);
		friend std::string operator+(std::string, Complex);

		// print
		friend std::ostream& operator<<(std::ostream &strm, const Complex &z);

		// sum
		friend Complex operator+(Complex, Complex);
		friend Complex operator+(Complex, double);
		friend Complex operator+(double, Complex);

		// subtraction
		friend Complex operator-(Complex);
		friend Complex operator-(Complex, Complex);
		friend Complex operator-(Complex, double);
		friend Complex operator-(double, Complex);

		// multiplication
		friend Complex operator*(Complex, Complex);
		friend Complex operator*(Complex, double);
		friend Complex operator*(double, Complex);

		// division
		friend Complex operator/(Complex, Complex);
		friend Complex operator/(Complex, double);
		friend Complex operator/(double, Complex);
		
		// equality
		friend bool operator==(Complex, Complex);
		friend bool operator==(Complex, double);
		friend bool operator==(double, Complex);
		
		// disequality
		friend bool operator!=(Complex, Complex);
		friend bool operator!=(Complex, double);
		friend bool operator!=(double, Complex);
};
