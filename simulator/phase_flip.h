#pragma once

#include "qstate.h"

QState pf_dencoder(QState, const char*);
QState pf_encoder(QState);
QState pf_decoder(QState);
QState pf_corrector(QState);
