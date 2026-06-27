#pragma once

#include <iostream>
#include <vector>

#include "complex.h"
#include "qstate.h"

class QGate{
	public:
		int qubits;
		std::vector<std::vector<Complex>> elems;
		const char* save_folder = "simulator/precompiled_gates";
		
		// constructor and destructor
		QGate(int = 0);
		~QGate();
		
		// save and load
		void save(const char*, int = 15);
		void load(const char*);
		
		// print
		friend std::ostream& operator<<(std::ostream &strm, const QGate &g);
		std::vector<Complex>& operator[](int);
		
		// conjugate
		QGate ct();
		
		// addition and subtraction
		friend QGate operator+(QGate, QGate);
		friend QGate operator-(QGate);
		friend QGate operator-(QGate, QGate);
		
		// multiplication
		friend QGate operator*(Complex, QGate);
		friend QGate operator*(QGate, Complex);
		friend QGate operator*(double, QGate);
		friend QGate operator*(QGate, double);
		friend QGate operator*(QGate, QGate);
		
		// gate application and measure
		friend QState operator*(QGate, QState);
		int measure(QState&);
		
		// tensor product
		friend QGate operator%(QGate, QGate);
};
