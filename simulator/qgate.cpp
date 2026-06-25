#include <iostream>
#include <fstream> 
#include <cmath>
#include <vector>
#include <numeric>
#include <filesystem>
#include <string>

#include "utilities.h"
#include "complex.h"
#include "qstate.h"
#include "qdgate.h"

// constructor and destructor
QGate::QGate(int qubits) : qubits(qubits) {
	// initialize all zeros
	if (qubits != 0){
		for(int i = 0; i < pow(2, qubits); i++){
			std::vector<Complex> row(pow(2, qubits),  Complex(0, 0));
			elems.push_back(row);
		}
	}
}
QGate::~QGate(){
	// free memory with the swap tricks
	std::vector<std::vector<Complex>>().swap(elems);
}

// save and load
void QGate::save(const char* gate_name, int precision){
	if (!std::filesystem::exists(save_folder)){
    	std::filesystem::create_directory(save_folder);
	}
	std::string filename = std::string(save_folder) + "/" + gate_name + ".txt";
  	std::ofstream save_file(filename);

	// write file
	save_file << "QUBITS: " << qubits << "\n";
	for (std::vector<Complex> row : elems){
		for (Complex z : row) save_file << std::setprecision(precision) << z.r << "\n" << std::setprecision(precision) << z.i << "\n";
	}
	// close file
	save_file.close();
}

void QGate::load(const char* gate_name){
	std::string filename = std::string(save_folder) + "/" + gate_name + ".txt";
	if (!std::filesystem::exists(filename)){
    	std::cerr << "ERROR: '" << filename << "' doesn't exist!" << std::endl;
	}
	// read file
	std::string line;
	std::ifstream load_file(filename);
	
	// save qubits
	std::getline(load_file, line);
	qubits = std::stoi(std::string(1, line[8]));
	
	// save elems
	elems.clear();
	for(int i = 0; i < pow(2, qubits); i++){
		std::vector<Complex> row;
		for(int j = 0; j < pow(2, qubits); j++){
			std::getline(load_file, line);
			double r = std::stod(line);
			
			std::getline(load_file, line);
			double i = std::stod(line);
			
			row.push_back(Complex(r, i));
		}
		elems.push_back(row);
	}
	
	// close file
	load_file.close();
}

// iterator and print
std::vector<Complex>& QGate::operator[](int i){
	return elems[i];
}

std::ostream& operator<<(std::ostream &strm, const QGate &g) {
	std::string output;
	for (std::vector<Complex> row : g.elems){
		output += "|";
		for (Complex z : row) output += " " + z + " ";
		output += "|\n";
	}
  	return strm << output;
}

// conjugate
QGate QGate::ct(){
	QGate transpose(qubits);
	for (int i = 0; i < pow(2, qubits); i++){
		for (int j = 0; j < pow(2, qubits); j++) {
			transpose[j][i] = elems[i][j].cc();
		}
	}
	return transpose;
}
// addition and subtraction
QGate operator+(QGate g1, QGate g2){
	QGate new_g(g1.qubits);
	for (int i = 0; i < pow(2, g1.qubits); i++){
		for (int j = 0; j < pow(2, g1.qubits); j++) {
			new_g[i][j] = g1[i][j] + g2[i][j];
		}
	}
	return new_g;
}

QGate operator-(QGate g){
	QGate new_g(g.qubits);
	for (int i = 0; i < pow(2, g.qubits); i++){
		for (int j = 0; j < pow(2, g.qubits); j++) new_g[i][j] = -g[i][j];
	}
	return new_g;
}

QGate operator-(QGate g1, QGate g2){
	return g1 + (-g2);
}

// multiplication
QGate operator*(Complex z, QGate g){
	QGate new_g(g.qubits);
	for (int i = 0; i < pow(2, g.qubits); i++){
		for (int j = 0; j < pow(2, g.qubits); j++) new_g[i][j] = z * g[i][j];
	}
	return new_g;
}

QGate operator*(QGate g, Complex z){
	return z * g;
}

QGate operator*(double n, QGate g){
	return Complex(n, 0) * g;
}

QGate operator*(QGate g, double n){
	return n * g;
}

QGate operator*(QGate g1, QGate g2){
	if (g1.qubits != g2.qubits){
		std::cerr << "ERROR: You can only multiplie same-size QGates!" << std::endl;
	}
	QGate new_g(g1.qubits);
	for (int i = 0; i < pow(2, g1.qubits); i++){
		for (int j = 0; j < pow(2, g1.qubits); j++) {
			Complex sum(0, 0);
			for (int k = 0; k < pow(2, g1.qubits); k++) sum = sum + g1[i][k] * g2[k][j];
			new_g[i][j] = sum;
		}
	}
	return new_g;
}

// gate application and measure
QState operator*(QGate g, QState s){
	if (g.qubits != s.qubits){
		std::cerr << "ERROR: You can only apply a QGate on a QState with same number of qubits!" << std::endl;
	}
	QState new_s(s.qubits);					// QGates are nxn matrices
	for (int i = 0; i < pow(2, s.qubits); i++) {
		new_s[i] = std::inner_product(g[i].begin(), g[i].end(), s.coeffs.begin(), Complex(0, 0));
	}
	return new_s;
}

int QGate::measure(QState& s) {    
	static QGate u = QG_Unity();
	static QGate U;
	if (U.qubits != qubits) U = wire_gate(u, qubits);
	
	static QGate proj_plus;
	proj_plus = 0.5 * (U + (*this));
	
	// measurement
	double prob = s.prod(proj_plus * s).r;
	int eigenvalue = (myrand() <= prob) ? 1 : -1;

	// projection
	s = (eigenvalue == 1) ? proj_plus * s : 0.5 * (U - (*this)) * s;
	s.normalizer();
	
	return eigenvalue;
}

// tensor product
QGate operator%(QGate g1, QGate g2){
	QGate new_g(g1.qubits + g2.qubits);
	int n = pow(2, g1.qubits);
	int m = pow(2, g2.qubits);
	for (int i = 0; i < pow(2, new_g.qubits); i++){
		for (int j = 0; j < pow(2, new_g.qubits); j++) {
			new_g[i][j] = g1[i / m][j / m] * g2[i % m][j % m];
		}
	}
	return new_g;
}
