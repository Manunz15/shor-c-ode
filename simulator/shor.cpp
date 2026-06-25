#include <iostream>

#include "utilities.h"
#include "error.h"
#include "complex.h"
#include "qstate.h"
#include "qdstate.h"
#include "qgate.h"
#include "qdgate.h"

QState shor_encoder(QState s){
	if (s.qubits != 9) s = add_zeros(s, 9);
	
	static QGate encoder;
	static bool loaded = false;
	if (!loaded){
		encoder.load("Encoder");
		loaded = true;
	}
	return encoder * s;
}
QState shor_decoder(QState s){
	static QGate decoder;
	static bool loaded = false;
	if (!loaded){
		decoder.load("Decoder");
		loaded = true;
	}
	return decoder * s;
}

QState shor_corrector(QState s){
	// initialization
	static bool loaded = false;
	static QGate Z12;
	static QGate Z23;
	static QGate Z45;
	static QGate Z56;
	static QGate Z78;
	static QGate Z89;
	static QGate X16;
	static QGate X49;
	static QGate Z123;
	static QGate Z456;
	static QGate Z789;
	
	if (!loaded){
		X16.load("X16");
		X49.load("X49");
		
		Z12.load("Z12");
		Z23.load("Z23");
		Z45.load("Z45");
		Z56.load("Z56");
		Z78.load("Z78");
		Z89.load("Z89");
		
		Z123.load("Z123");
		Z456.load("Z456");
		Z789.load("Z789");
		
		loaded = true;
	}
	
	// bit flip
	static QGate Zs[6] = {Z12, Z23, Z45, Z56, Z78, Z89};
	for (int i = 0; i < 3; i++){	
		int z1 = Zs[2 * i].measure(s);
		int z2 = Zs[2 * i + 1].measure(s);
		
		if (z1 == -1 && z2 == 1) s = error(s, 3 * i + 1, "bit");
		else if (z1 == -1 && z2 == -1) s = error(s, 3 * i + 2, "bit");
		else if (z1 == 1 && z2 == -1) s = error(s, 3 * i + 3, "bit");
	}
	
	// phase flip
	int z16 = X16.measure(s); 
	int z49 = X49.measure(s); 
	
	if (z16 == -1 && z49 == 1) s = Z123 * s;
	else if (z16 == -1 && z49 == -1) s = Z456 * s;
	else if (z16 == 1 && z49 == -1) s = Z789 * s;
	
	return s;
}
