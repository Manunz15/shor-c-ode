#include <iostream>
#include <cmath>

#include "utilities.h"
#include "complex.h"
#include "qstate.h"
#include "qdstate.h"
#include "qgate.h"
#include "qdgate.h"

QState bit_flip(QState s, int pos){
	// initialization
	static bool initialized = false;
	static QGate x = QG_SigmaX();
	static QGate bit_flips[3];
	
	if (!initialized){
		initialized = true;
		for (int i = 0; i < 3; i++){
			bit_flips[i] = wire_gate(x, 3, i + 1);
		}
	}
	// 1-qubits QStates
	if (s.qubits == 1) return x * s;
	
	// choose random qubit
	if (pos == -1) pos = myrandint(1, 3);
	return bit_flips[pos - 1] * s;
}

QState phase_flip(QState s, int pos){
	// initialization
	static bool initialized = false;
	static QGate z = QG_SigmaZ();
	static QGate phase_flips[3];
	
	if (!initialized){
		initialized = true;
		for (int i = 0; i < 3; i++){
			phase_flips[i] = wire_gate(z, 3, i + 1);
		}
	}
	// 1-qubits QStates
	if (s.qubits == 1) return z * s;
	
	// choose random qubit
	if (pos == -1) pos = myrandint(1, 3);
	
	return phase_flips[pos - 1] * s;
}

QState error(QState s, int pos, const char* type){
	// initialization
	static bool initialized = false;
	static QGate u = QG_Unity();
	static QGate U = wire_gate(u, 9);
	static QGate x = QG_SigmaX();
	static QGate z = QG_SigmaZ();
	static QGate iy = x * z;
	static QGate bit_flips[9];
	static QGate phase_flips[9];
	static QGate xz_flips[9];
	
	if (!initialized){
		initialized = true;
		for (int i = 0; i < 9; i++){
			bit_flips[i] = wire_gate(x, 9, i + 1);
			phase_flips[i] = wire_gate(z, 9, i + 1);
			xz_flips[i] = wire_gate(iy, 9, i + 1);
		}
	}

	// choose random qubit
	if (pos == -1) pos = myrandint(1, 9);

	// apply error
	if (std::string(type) == "bit") return bit_flips[pos - 1] * s;
	else if (std::string(type) == "phase") return phase_flips[pos - 1] * s;
	else{
		Complex alpha = Complex(cos(myrand() * 2 * M_PI), sin(myrand() * 2 * M_PI)) / 2;
		Complex beta = Complex(cos(myrand() * 2 * M_PI), sin(myrand() * 2 * M_PI)) / 2;
		Complex gamma = Complex(cos(myrand() * 2 * M_PI), sin(myrand() * 2 * M_PI)) / 2;
		Complex delta = Complex(cos(myrand() * 2 * M_PI), sin(myrand() * 2 * M_PI)) / 2;
		return (alpha * U + beta * bit_flips[pos - 1] + gamma * phase_flips[pos - 1]+ delta * xz_flips[pos - 1]) * s;
	}
}

