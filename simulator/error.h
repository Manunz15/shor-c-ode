#pragma once

#include "qstate.h"

QState bit_flip(QState, int = -1);
QState phase_flip(QState, int = -1);
QState error(QState, int = -1, const char* = "general");
