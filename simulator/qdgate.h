#pragma once 

#include <cmath>

#include "utilities.h"
#include "complex.h"
#include "qgate.h"

// zero and one
class QG_ZeroZero : public QGate {
	public:
		// constructor
		QG_ZeroZero() : QGate(1) {
			elems[0][0] = Complex(1, 0);
		};
};
class QG_OneOne : public QGate {
	public:
		// constructor
		QG_OneOne() : QGate(1) {
			elems[1][1] = Complex(1, 0);
		};
};

// pauli matrices
class QG_Unity : public QGate {
	public:
		// constructor
		QG_Unity() : QGate(1) {
			elems[0][0] = Complex(1, 0);
			elems[1][1] = Complex(1, 0);
		};
};
class QG_SigmaX : public QGate {
	public:
		// constructor
		QG_SigmaX() : QGate(1) {
			elems[0][1] = Complex(1, 0);
			elems[1][0] = Complex(1, 0);
		};
};
class QG_SigmaY : public QGate {
	public:
		// constructor
		QG_SigmaY() : QGate(1) {
			elems[0][1] = Complex(0, -1);
			elems[1][0] = Complex(0, 1);
		};
};
class QG_SigmaZ : public QGate {
	public:
		// constructor
		QG_SigmaZ() : QGate(1) {
			elems[0][0] = Complex(1, 0);
			elems[1][1] = Complex(-1, 0);
		};
};

// specials
class QG_Hadamard : public QGate {
	public:
		// constructor
		QG_Hadamard() : QGate(1) {
			elems[0][0] = Complex(1, 0) / sqrt(2);
			elems[0][1] = Complex(1, 0) / sqrt(2);
			elems[1][0] = Complex(1, 0) / sqrt(2);
			elems[1][1] = Complex(-1, 0) / sqrt(2);
		};
};
class QG_Phase : public QGate {
	public:
		// constructor
		QG_Phase() : QGate(1) {
			elems[0][0] = Complex(1, 0);
			elems[1][1] = Complex(0, 1);
		};
};
class QG_Pi8 : public QGate {
	public:
		// constructor
		QG_Pi8() : QGate(1) {
			elems[0][0] = Complex(1, 0);
			elems[1][1] = Complex(1, 1) / sqrt(2);
		};
};

// cnot
class QG_CNOT : public QGate {
	public:
		// constructor
		QG_CNOT(int wires = 2, int pos_point = 1, int pos_cross = 2) : QGate(wires) {
			QGate x = QG_SigmaX();
			QGate cnot = control_gate(x, wires, pos_point, pos_cross);
			elems = cnot.elems;
		};
};

// toffoli
class QG_CCNOT : public QGate {
	public:
		// constructor
		QG_CCNOT(int wires = 3, int pos_point1 = 1, int pos_point2 = 2, int pos_cross = 3) : QGate(wires) {
			int pos1 = std::min(pos_point1, pos_point2);
			int pos2 = std::max(pos_point1, pos_point2);
			QGate x = QG_SigmaX();
			QGate toffoli(wires);
			
			// if cross above
			if (pos_cross < pos1) {
				QGate cnot = control_gate(x, pos1, pos1, pos_cross);
				toffoli = control_gate(cnot, wires - pos1 + 1, pos2 - pos1 + 1, 1);
			}
			// if cross below
			else if (pos_cross > pos2) {
				QGate cnot = control_gate(x, wires - pos2 + 1, 1, pos_cross - pos2 + 1);
				toffoli = control_gate(cnot, pos2, pos1, pos2);
			}
			// if cross between
			else {
				QGate cnot = control_gate(x, pos_cross, pos1, pos_cross);
				toffoli = control_gate(cnot, wires - pos_cross + 1, pos2 - pos_cross + 1, 1);
			}
			
			elems = toffoli.elems;
		};
};
