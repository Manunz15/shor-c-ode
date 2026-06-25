#pragma once

#include "complex.h"
#include "qstate.h"
#include "qgate.h"

// miscellaneus
std::string int_to_binary(int, int);

// math
double myrand();
int myrandint(int, int);
bool kind_equal(double, double, double = 0.001);

// QStates and QGates
QState add_zeros(const QState, int, int = 1);
QGate wire_gate(const QGate, int, int = 1);
QGate control_gate(QGate, int, int = 1, int = 2);

// simulators
QState rand_error(QState, double, const char* = "general");
bool noenc_sim(double, double);
bool bf_sim(double);
bool pf_sim(double);
bool shor_sim(double, double);
