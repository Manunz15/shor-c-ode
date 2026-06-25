#include <iostream>
#include "utilities.h"

#include "qgate.h"
#include "qdgate.h"

int main(){
	int w = 9;
	QGate H = QG_Hadamard();
	
	// ENCODER 
	QGate cnot14 = QG_CNOT(w, 1, 4);
	QGate cnot17 = QG_CNOT(w, 1, 7);
	QGate H1 = wire_gate(H, w, 1);
	QGate H4 = wire_gate(H, w, 4);
	QGate H7 = wire_gate(H, w, 7);
	QGate cnot12 = QG_CNOT(w, 1, 2);
	QGate cnot13 = QG_CNOT(w, 1, 3);
	QGate cnot45 = QG_CNOT(w, 4, 5);
	QGate cnot46 = QG_CNOT(w, 4, 6);
	QGate cnot78 = QG_CNOT(w, 7, 8);
	QGate cnot79 = QG_CNOT(w, 7, 9);
	
	// QGate encoder = cnot12 * cnot13 * cnot45 * cnot46 * cnot78 * cnot79 * H1 * H4 * H7 * cnot14 * cnot17;
	// encoder.save("Encoder");
	
	// DECODER
	QGate ccnot123_i = QG_CCNOT(w, 2, 3, 1);
	QGate ccnot456_i = QG_CCNOT(w, 5, 6, 4);
	QGate ccnot789_i = QG_CCNOT(w, 8, 9, 7);
	QGate ccnot147_i = QG_CCNOT(w, 4, 7, 1);
	
	// QGate decoder = ccnot147_i * cnot17 * cnot14 * H1 * H4 * H7 * ccnot123_i * ccnot456_i * ccnot789_i * cnot12 * cnot13 * cnot45 * cnot46 * cnot78 * cnot79;
	// decoder.save("Decoder");
	
	// X PROJECTORS
	QGate x = QG_SigmaX();
	QGate X1 = wire_gate(x, w, 1);
	QGate X2 = wire_gate(x, w, 2);
	QGate X3 = wire_gate(x, w, 3);
	QGate X4 = wire_gate(x, w, 4);
	QGate X5 = wire_gate(x, w, 5);
	QGate X6 = wire_gate(x, w, 6);
	QGate X7 = wire_gate(x, w, 7);
	QGate X8 = wire_gate(x, w, 8);
	QGate X9 = wire_gate(x, w, 9);
	
	QGate X12 = X1 * X2;
	QGate X23 = X2 * X3;
	QGate X45 = X4 * X5;
	QGate X56 = X5 * X6;
	QGate X78 = X7 * X8;
	QGate X89 = X8 * X9;
	X12.save("X12");
	X23.save("X23");
	X45.save("X45");
	X56.save("X56");
	X78.save("X78");
	X89.save("X89");
	
	QGate X16 = X1 * X2 * X3 * X4 * X5 * X6;
	QGate X49 = X4 * X5 * X6 * X7 * X8 * X9;
	X16.save("X16");
	X49.save("X49");
	
	// Z PROJECTORS
	QGate z = QG_SigmaZ();
	QGate Z1 = wire_gate(z, w, 1);
	QGate Z2 = wire_gate(z, w, 2);
	QGate Z3 = wire_gate(z, w, 3);
	QGate Z4 = wire_gate(z, w, 4);
	QGate Z5 = wire_gate(z, w, 5);
	QGate Z6 = wire_gate(z, w, 6);
	QGate Z7 = wire_gate(z, w, 7);
	QGate Z8 = wire_gate(z, w, 8);
	QGate Z9 = wire_gate(z, w, 9);
	
	QGate Z123 = Z1 * Z2 * Z3;
	QGate Z456 = Z4 * Z5 * Z6;
	QGate Z789 = Z7 * Z8 * Z9;
	
	Z123.save("Z123");
	Z456.save("Z456");
	Z789.save("Z789");
	
	QGate Z12 = Z1 * Z2;
	QGate Z23 = Z2 * Z3;
	QGate Z45 = Z4 * Z5;
	QGate Z56 = Z5 * Z6;
	QGate Z78 = Z7 * Z8;
	QGate Z89 = Z8 * Z9;
	Z12.save("Z12");
	Z23.save("Z23");
	Z45.save("Z45");
	Z56.save("Z56");
	Z78.save("Z78");
	Z89.save("Z89");
	
	return 0;
}
