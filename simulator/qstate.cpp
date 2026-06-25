#include <cmath>

#include "complex.h"
#include "utilities.h"

// constructor and destructor
QState::QState(int qubits) : qubits(qubits) {
	// initialize all zeros
	coeffs = std::vector<Complex>(pow(2, qubits),  Complex(0, 0));
}
QState::~QState(){
	// free memory with the swap tricks
	std::vector<Complex>().swap(coeffs);
}

// initializers
void QState::normalizer(){
	double norm2 = (*this).norm2();
	if (norm2 != 1){
		for (int i = 0; i < coeffs.size(); i++) coeffs[i] = coeffs[i] / sqrt(norm2);
	}
}

void QState::random(){
	for (int i = 0; i < pow(2, qubits); i++) coeffs[i] = Complex(myrand(), myrand());
	normalizer();
}

// iterator and print
Complex& QState::operator[](int i){
	return coeffs[i];
}

std::ostream& operator<<(std::ostream &strm, const QState &s) {
	std::string output;
	int count = 0;
	int addends = 0;
	for (Complex z : s.coeffs){
		if (z != 0){
			if (addends  > 0) output += " + ";
			output += "(" + z + ")|" + int_to_binary(count, s.qubits) + ">";
			addends++;
		}
		count++;
	}
  	return strm << output;
}

void QState::print_probs(){
	std::string output;
	int count = 0;
	int addends = 0;
	for (Complex z : coeffs){
		if (z != 0){
			if (addends  > 0) output += ", ";
			output += std::to_string(z.m2()) + "% |" + int_to_binary(count, qubits) + ">";
			addends++;
		}
		count++;
	}
	std::cout << output << std::endl;
}

// scalar product and norm
Complex QState::prod(QState s){
	if (qubits != s.qubits) std::cerr << "ERROR: Only same-size QStates have a scalar product!" << std::endl;
	Complex prod(0, 0);
	for (int i = 0; i < pow(2, qubits); i++) prod = prod + coeffs[i].cc() * s[i];
  	return prod;
}

double QState::norm2(){
  	return (*this).prod(*this).r;
}

// multiplication
QState operator*(QState s1, QState s2){
	QState s3(s1.qubits + s2.qubits);
	int count = 0;
	for (int i = 0; i < pow(2, s1.qubits); i++){
			for (int j = 0; j < pow(2, s2.qubits); j++){
				s3[count] = s1[i] * s2[j];
				count++;
		}
	}
  	return s3;
}

QState operator*(QState s, Complex z){
	QState new_s(s.qubits);
	for (int i = 0; i < pow(2, s.qubits); i++) new_s[i] = s[i] * z;
  	return new_s;
}

QState operator*(Complex z, QState s){
  	return s * z;
}

// equality
bool operator==(QState s1, QState s2){
	if (s1.qubits != s2.qubits) return false;

	Complex phase = s2[0] / s1[0];
	for (int i = 1; i < pow(2, s1.qubits); i++) {
		if (!(s1[i] == s2[i] / phase)) return false;
	}
  	return true;
}
bool operator==(QState s, double n){
	if (n != 0) return false;
	for (Complex z : s.coeffs) {
		if (z != 0) return false;
	}
  	return true;
}
bool operator==(double n, QState s){
  	return s == n;
}

// disequality
bool operator!=(QState s1, QState s2){
  	return !(s1 == s2);
}
bool operator!=(QState s, double n){
  	return !(s == n);
}
bool operator!=(double n, QState s){
  	return !(s == n);
}
