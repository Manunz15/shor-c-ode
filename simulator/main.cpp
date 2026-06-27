#include <iostream>
#include <string>
#include <TH1D.h>
#include <TFile.h>
#include <TH2D.h>
#include <TCanvas.h>
#include <TROOT.h>

#include "complex.h"
#include "utilities.h"
#include "error.h"

#include "qstate.h"
#include "qdstate.h"

#include "qgate.h"
#include "qdgate.h"

#include "bit_flip.h"
#include "phase_flip.h"
#include "shor.h"

void sim_1D(bool(double), double, int, double, const char*);
void sim_2D(bool(double, double), double, int, double, int, double, const char*);

int main(){  
	// CAUTION: THIS CODE WORK ONLY IF "MAIN.EXE" IS OUTSIDE THE FOLDER SIMULATOR/
	
	// IN THIS FILE THERE ARE THE CODE FOR SIMULATION (COMMENTED) AND AN EXAMPLE (UNCOMMENTED).
	
	double pmax = 1;
	double qmax = 1;
	const int ppart = 50;
	const int qpart = 50;
	double num = 1000;
	
	// THIS CODE WAS USED FOR SIMULATIONS
	
	// sim_1D(bf_sim, pmax, ppart, num, "bit_flip");
	// sim_1D(pf_sim, qmax, qpart, num, "phase_flip");
	// sim_2D(noenc_sim, pmax, ppart, qmax, qpart, num, "no_encoding");
	// sim_1D([](double p) -> bool {return noenc_sim(p, 0);}, pmax, ppart, num, "no_encoding_p");
	// sim_1D([](double q) -> bool {return noenc_sim(0, q);}, qmax, qpart, num, "no_encoding_q");
	
	
	// THIS IS SHOR CODE, CAUTION: IT CAN TAKE HOURS BEFORE ENDING A SIMULATION
	
	// sim_1D([](double p) -> bool {return shor_sim(p, 0);}, pmax, ppart, num, "shor_p");
	// sim_1D([](double q) -> bool {return shor_sim(0, q);}, qmax, qpart, num, "shor_q");
        
	QState s(1);
	s.random();
	
	std::cout << "A random quantum state: "<< s << std::endl;
	
	std::cout << "\n---------\nBIT FLIPS\n---------"<< std::endl;
	
	QState enc_s1 = bf_encoder(s);     
	std::cout << "\nThe encoded state: "<< enc_s1 << std::endl;
	QState err_s1 = bit_flip(enc_s1, 2);
	std::cout << "\nBit flips on the second qubit: "<< err_s1 << std::endl;
	QState corr_s1 = bf_corrector(err_s1);
	std::cout << "\nThe corrected state: "<< corr_s1 << std::endl;
	QState dec_s1 = bf_decoder(corr_s1);
	std::cout << "\nThe decoded state (the original one multiplied by |00>): "<< dec_s1 << std::endl;
	
	std::cout << "\n-----------\nPHASE FLIPS\n-----------"<< std::endl;
	
	QState enc_s2 = pf_encoder(s);     
	std::cout << "\nThe encoded state: "<< enc_s2 << std::endl;
	QState err_s2 = phase_flip(enc_s2, 1);
	std::cout << "\nPhase flips on the first qubit: "<< err_s2 << std::endl;
	QState corr_s2 = pf_corrector(err_s2);
	std::cout << "\nThe corrected state: "<< corr_s2 << std::endl;
	QState dec_s2 = pf_decoder(corr_s2);
	std::cout << "\nThe decoded state (the original one multiplied by |00>): "<< dec_s2 << std::endl;
	
	
	std::cout << "\n-----------\nSHOR CODE\n-----------"<< std::endl;
	
	QState enc_s3 = shor_encoder(s);     
	std::cout << "\nThe encoded state: "<< enc_s3 << std::endl;
	QState err_s3 = error(enc_s3, 1);
	std::cout << "\nGeneral error on the first qubit: "<< err_s3 << std::endl;
	QState corr_s3 = shor_corrector(err_s3);
	std::cout << "\nThe corrected state: "<< corr_s3 << std::endl;
	QState dec_s3 = shor_decoder(corr_s3);
	std::cout << "\nThe decoded state: "<< dec_s3 << std::endl;
	std::cout << "\nThis is equal to the original state within a phase: (corr_s == enc_s) = "<< (bool)(corr_s3 == enc_s3)<< std::endl;
	return 0;
}

void sim_1D(bool (*func)(double), double rmax, int part, double num, const char* title){
	double hmin = - rmax / (2 * part);
	double hmax = rmax + rmax / (2 * part);
	
	TH1D h = TH1D(title, title, part + 1, hmin, hmax);
	for (int i = 0; i <= part; i++){
		std::cout << 100 * i / part << "%" << std::endl;
		
		double r = i * rmax / part;	
		for (int k = 0; k < num; k++) if (func(r)) h.Fill(r);
	}
	std::string filename = "hist/" + std::string(title) + ".root";
	TFile f = TFile(filename.c_str(), "RECREATE");
	h.Write();
	f.Close();
}

void sim_2D(bool (*func)(double, double), double pmax, int ppart, double qmax, int qpart, double num, const char* title){
	double phmin = - pmax / (2 * ppart);
	double phmax = pmax + pmax / (2 * ppart);
	double qhmin = - qmax / (2 * qpart);
	double qhmax = qmax + qmax / (2 * qpart);
	
	TH2D h = TH2D(title, title, ppart + 1, phmin, phmax, qpart + 1, qhmin, qhmax);
	for (int i = 0; i <= ppart; i++){
		std::cout << 100 * i / ppart << "%" << std::endl;
		
		double p = i * pmax / ppart;	
		for (int j = 0; j <= qpart; j++){
			double q = j * qmax / qpart;	
			for (int k = 0; k < num; k++) if (func(p, q)) h.Fill(p, q);
		}
	}
	std::string filename = "hist/" + std::string(title) + ".root";
	TFile f = TFile(filename.c_str(), "RECREATE");
	h.Write();
	f.Close();
}
