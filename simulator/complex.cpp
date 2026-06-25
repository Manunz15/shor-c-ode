#include <cmath>
#include <sstream>
#include <iomanip>

#include "utilities.h"

// constructor
Complex::Complex(double r, double i) : r(r), i(i) {}

// complex conjugate, module and power
Complex Complex::cc(){
	return Complex(r, -i);
}

double Complex::m(){
  	return sqrt(pow(r, 2) + pow(i, 2));
}

double Complex::m2(){
	return pow(r, 2) + pow(i, 2);
}

Complex Complex::cpow(int n){
	Complex z = Complex(r, i);
	Complex zi = Complex(r, i);
	for(int i = 1; i < n; i++)z = z * zi;
	return z;
}

// string
std::string Complex::str(int precision = 2) const {
	// ' ' is a char, " " a const char*
	std::ostringstream oss;
	if (i != 0 and r != 0){
		std::string sign = (i > 0) ? "+" : "";
		oss << std::fixed << std::setprecision(precision) << r << sign << std::fixed << std::setprecision(precision) << i << "j";
	} 
	else if (i != 0){
			oss << std::fixed << std::fixed << std::setprecision(precision) << i << "j";
	}
	else{
		oss << std::fixed << std::setprecision(precision) << r;
	}
	std::string written = oss.str();
	return written;
}
std::string operator+(Complex z, std::string str){
	return z.str() + str;
}
std::string operator+(std::string str, Complex z){
	return str + z.str();
}
// print
std::ostream& operator<<(std::ostream &strm, const Complex &z) {
	Complex z1(z.r, z.i);
	return strm << z1.str();
}

// sum
Complex operator+(Complex z1, Complex z2){
  	return Complex(z1.r + z2.r, z1.i + z2.i);
}
Complex operator+(Complex z, double n){
  	return Complex(z.r + n, z.i);
}
Complex operator+(double n, Complex z){
  	return z + n;
}

// subtraction
Complex operator-(Complex z){
  	return Complex(-z.r, -z.i);
}
Complex operator-(Complex z1, Complex z2){
  	return z1 + (-z2);
}
Complex operator-(Complex z, double n){
  	return z + (-n);
}
Complex operator-(double n, Complex z){
  	return n + (-z);
}

// multiplication
Complex operator*(Complex z1, Complex z2){
  	return Complex(z1.r * z2.r - z1.i * z2.i, z1.r * z2.i + z1.i * z2.r);
}
Complex operator*(Complex z, double n){
  	return Complex(z.r * n, z.i * n);
}
Complex operator*(double n, Complex z){
  	return z * n;
}

// division
Complex operator/(Complex z1, Complex z2){
  	return z1 * z2.cc() / z2.m2();
}
Complex operator/(Complex z, double n){
  	return z * (1 / n);
}
Complex operator/(double n, Complex z){
  	return n * z.cc() / z.m2();
}

// equality
bool operator==(Complex z1, Complex z2){
  	return kind_equal(z1.r, z2.r) && kind_equal(z1.i, z2.i);
}
bool operator==(Complex z, double n){
  	return kind_equal(z.r, n) && kind_equal(z.i, 0);
}
bool operator==(double n, Complex z){
  	return z == n;
}

// disequality
bool operator!=(Complex z1, Complex z2){
  	return !(z1 == z2);
}
bool operator!=(Complex z, double n){
  	return !(z == n);
}
bool operator!=(double n, Complex z){
  	return !(z == n);
}
