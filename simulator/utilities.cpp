#include <bitset>
#include <TRandom3.h>
#include <cmath>

#include "qstate.h"
#include "qdstate.h"
#include "qgate.h"
#include "qdgate.h"
#include "error.h"
#include "bit_flip.h"
#include "phase_flip.h"
#include "shor.h"

// miscellaneus
std::string int_to_binary(int n, int digits){
	// it converts an integer to its binary representation
	std::string binary = std::bitset<64>(n).to_string().substr(64 - digits);
	return binary;
}

// math
double myrand(){
	static TRandom3 *newrand = new TRandom3();
	static bool initialized = false;
	if (!initialized){
		initialized = true;
		newrand->SetSeed(42);
		gRandom = newrand;
	}
	return newrand->Rndm();
}

int myrandint(int min, int max){
	// gives random integers in [min, max]
	double rand = myrand();
	return (int) (rand * (max + 1) + (1 - rand) * min);
}

bool kind_equal(double n1, double n2, double epsilon){
	// equality with small error tollerance
	return std::abs(n1 - n2) < epsilon;
}

// QStates and QGates
QState add_zeros(const QState s, int wires, int pos){
	QState new_s = s;
	static QState o = QS_Zero();
	for (int i = 1; i < pos; i++) new_s = o * new_s;
	for (int i = pos; i < wires; i++) new_s = new_s * o;
	return new_s;
}

QGate wire_gate(const QGate g, int wires, int pos){
	QGate new_g = g;
	static QGate u = QG_Unity();
	for (int i = 1; i < pos; i++) new_g = u % new_g;
	for (int i = pos; i < wires; i++) new_g = new_g % u;
	return new_g;
}

QGate control_gate(const QGate g, int wires, int pos_point, int pos_cross){
	QGate new_g = g;
	QGate oo = QG_ZeroZero();
	QGate ll = QG_OneOne();
	QGate U = QG_Unity();
	static QGate u = QG_Unity();
	
	// identity of final dimension
	for (int i = 1; i < new_g.qubits; i++) U = U % u;
	
	// if cross below
	if (pos_cross > pos_point){
		for (int i = pos_point; i < pos_cross - 1; i++){
			oo = oo % u;
			ll = ll % u;
		}
		new_g = (oo % U) + (ll % new_g);
	}
	// if cross above
	else {
			for (int i = pos_cross; i < pos_point - 1; i++){
			oo = u % oo;
			ll = u % ll;
		}
		new_g = (U % oo) + (new_g % ll);
	}
	for (int i = 1; i < fmin(pos_point, pos_cross); i++) new_g = u % new_g;
	for (int i = fmax(pos_point, pos_cross); i < wires; i++) new_g = new_g % u;
	return new_g;
}

// simulators
QState rand_error(QState s, double p, const char* type){
	for (int i = 1; i <= s.qubits; i++){
		if (p > myrand()){
			if (s.qubits == 9) s = error(s, i, type);
			else if (std::string(type) == "bit") s = bit_flip(s, i);
			else if (std::string(type) == "phase") s = phase_flip(s, i);
		}
	}
	return s;
}

bool noenc_sim(double p, double q){
	QState s(1);
	s.random();
	
	QState err_s = rand_error(s, p, "bit");
	err_s = rand_error(err_s, q, "phase");
	if (err_s == s) return true;
	else return false;
}

bool bf_sim(double p){
	QState s(1);
	s.random();
	
	// encoding
	QState enc_s = bf_encoder(s);
	QState err_s = rand_error(enc_s, p, "bit");
	QState corr_s = bf_corrector(err_s);
	
	if (corr_s == enc_s) return true;
	else return false;
}

bool pf_sim(double q){
	QState s(1);
	s.random();
	
	// encoding
	QState enc_s = pf_encoder(s);
	QState err_s = rand_error(enc_s, q, "phase");
	QState corr_s = pf_corrector(err_s);
	
	if (corr_s == enc_s) return true;
	else return false;
}

bool shor_sim(double p, double q){
	QState s(1);
	s.random();
	
	// encoding
	QState enc_s = shor_encoder(s);
	QState err_s = rand_error(enc_s, p, "bit");
	err_s = rand_error(err_s, q, "phase");
	QState corr_s = shor_corrector(err_s);
	
	if (corr_s == enc_s) return true;
	else return false;
}
