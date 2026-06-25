#pragma once 

#include <cmath>

#include "complex.h"
#include "qstate.h"

// computational basis
class QS_Zero : public QState {
	public:
		// constructor
		QS_Zero() : QState(1) {
			coeffs[0] = Complex(1, 0);
		};
};
class QS_One : public QState {
	public:
		// constructor
		QS_One() : QState(1) {
			coeffs[1] = Complex(1, 0);
		};
};

// hadamard basis
class QS_Plus : public QState {
	public:
		// constructor
		QS_Plus() : QState(1) {
			coeffs[0] = Complex(1, 0) / sqrt(2);
			coeffs[1] = Complex(1, 0) / sqrt(2);
		};
};
class QS_Minus : public QState {
	public:
		// constructor
		QS_Minus() : QState(1) {
			coeffs[0] = Complex(1, 0) / sqrt(2);
			coeffs[1] = Complex(-1, 0) / sqrt(2);
		};
};
