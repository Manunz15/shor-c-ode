#pragma once

#include <iostream>
#include <vector>

#include "complex.h"

class QState{
	public:
		int qubits;  					// number of qubits
		std::vector<Complex> coeffs;	// complex coefficients

		// constructor and destructor
		QState(int); 
		~QState();
		
		// initializers
		void normalizer();
		void random();

		// iterator and print
		Complex& operator[](int);
		friend std::ostream& operator<<(std::ostream &strm, const QState &s);
		void print_probs();
		
		// scalar product and norm
		Complex prod(QState);
		double norm2();			// this is squared norm
		
		// multiplication
		friend QState operator*(QState, QState);
		friend QState operator*(QState, Complex);
		friend QState operator*(Complex, QState);
		
		// equality
		friend bool operator==(QState, QState);
		friend bool operator==(QState, double);
		friend bool operator==(double, QState);
		
		// disequality
		friend bool operator!=(QState, QState);
		friend bool operator!=(QState, double);
		friend bool operator!=(double, QState);
};
