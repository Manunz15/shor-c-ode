#include <iostream>

#include "utilities.h"
#include "error.h"
#include "complex.h"
#include "qstate.h"
#include "qdstate.h"
#include "qgate.h"
#include "qdgate.h"

QState pf_dencoder(QState s, const char* function){
	if (s.qubits != 3) s = add_zeros(s, 3);
	
	static QGate x = QG_SigmaX();
	static QGate z = QG_SigmaZ();
	static QGate h = QG_Hadamard();
	
	// encoder and decoder
	static QGate cnot12 = QG_CNOT(3, 1, 2);
	static QGate cnot13 = QG_CNOT(3, 1, 3);
	static QGate H = wire_gate(h, 3, 1) * wire_gate(h, 3, 2) * wire_gate(h, 3, 3);
	static QGate encoder = H * cnot12 * cnot13;
	static QGate decoder = cnot12 * cnot13 * H;

	if (function == "encode") return encoder * s;
	else return decoder * s;
}

QState pf_encoder(QState s){
	return pf_dencoder(s, "encode");
}
QState pf_decoder(QState s){
	return pf_dencoder(s, "decode");
}

QState pf_corrector(QState s){
	// initialization
	static QGate x = QG_SigmaX();
	static QGate X1 = wire_gate(x, 3, 1);
	static QGate X2 = wire_gate(x, 3, 2);
	static QGate X3 = wire_gate(x, 3, 3);
	static QGate X1_X2 = X1 * X2;
	static QGate X2_X3 = X2 * X3;
	
	// detector
	int z1 = X1_X2.measure(s);
	int z2 = X2_X3.measure(s);
	
	if (z1 == -1 && z2 == 1) return phase_flip(s, 1);
	else if (z1 == -1 && z2 == -1) return phase_flip(s, 2);
	else if (z1 == 1 && z2 == -1) return phase_flip(s, 3);
	else return s;
}
