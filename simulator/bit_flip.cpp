#include <iostream>

#include "utilities.h"
#include "error.h"
#include "complex.h"
#include "qstate.h"
#include "qdstate.h"
#include "qgate.h"
#include "qdgate.h"

QState bf_dencoder(QState s){
	if (s.qubits != 3) s = add_zeros(s, 3);
	
	static QGate x = QG_SigmaX();
	static QGate z = QG_SigmaZ();
	
	// encoder and decoder (they are the same here)
	static QGate cnot12 = QG_CNOT(3, 1, 2);
	static QGate cnot13 = QG_CNOT(3, 1, 3);
	static QGate dencoder = cnot12 * cnot13;
	
	return dencoder * s;
}

QState bf_encoder(QState s){
	return bf_dencoder(s);
}
QState bf_decoder(QState s){
	return bf_dencoder(s);
}

QState bf_corrector(QState s){
	// initialization
	static QGate z = QG_SigmaZ();
	static QGate Z1 = wire_gate(z, 3, 1);
	static QGate Z2 = wire_gate(z, 3, 2);
	static QGate Z3 = wire_gate(z, 3, 3);
	static QGate Z1_Z2 = Z1 * Z2;
	static QGate Z2_Z3 = Z2 * Z3;
	
	// detector
	int z1 = Z1_Z2.measure(s);
	int z2 = Z2_Z3.measure(s);
	
	if (z1 == -1 && z2 == 1) return bit_flip(s, 1);
	else if (z1 == -1 && z2 == -1) return bit_flip(s, 2);
	else if (z1 == 1 && z2 == -1) return bit_flip(s, 3);
	else return s;
}
